/*
 *  Copyright (C) 2018-2024 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fx_api.h"
#include "lx_api.h"

#include <lx_nand_flash_driver.h>


VOID  _fx_nand_driver(FX_MEDIA *media_ptr)
{
    LX_NAND_FLASH *p_nand_flash;
    ULONG logical_sector;
    ULONG count;
    UCHAR *buffer;
    UINT status;
  
    switch(media_ptr -> fx_media_driver_request)
    {

        case FX_DRIVER_READ:
        {
            p_nand_flash = (LX_NAND_FLASH *)media_ptr->fx_media_driver_info;

            /* Read sector(s) from NAND flash.  */
            logical_sector =  media_ptr -> fx_media_driver_logical_sector;
            count =  media_ptr -> fx_media_driver_sectors;
            buffer = (UCHAR *) media_ptr -> fx_media_driver_buffer;
            while (count)
            {

                /* Call LevelX to read one flash sector.  */
                status =  _lx_nand_flash_sector_read(p_nand_flash, logical_sector, buffer);
                
                /* Determine if the read was successful.  */
                if (status != LX_SUCCESS)
                {
                
                    /* Return an I/O error to FileX.  */
                    media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
                    
                    return;
                } 

                /* Successful sector read.  */                
                count--;
                logical_sector++;
                buffer += media_ptr -> fx_media_bytes_per_sector;
            }
            /* Successful driver request.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }
        
        case FX_DRIVER_WRITE:
        {
            p_nand_flash = (LX_NAND_FLASH *)media_ptr->fx_media_driver_info;

            /* Write sector(s) to NAND flash.  */
            logical_sector =  media_ptr -> fx_media_driver_logical_sector;           
            count =  media_ptr -> fx_media_driver_sectors;
            buffer = (UCHAR *) media_ptr -> fx_media_driver_buffer;
            while (count)
            {

                /* Call LevelX to write a sector.  */
                status =  _lx_nand_flash_sector_write(p_nand_flash, logical_sector, buffer);

                /* Determine if the write was successful.  */
                if (status != LX_SUCCESS)
                {
                
                    /* Return an I/O error to FileX.  */
                    media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
                    
                    return;
                } 

                /* Successful sector write.  */
                count--;
                logical_sector++;
                buffer += media_ptr -> fx_media_bytes_per_sector;
            }

            /* Successful driver request.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        case FX_DRIVER_RELEASE_SECTORS:
        {
            p_nand_flash = (LX_NAND_FLASH *)media_ptr->fx_media_driver_info;
                   
            /* Release the mapping of this sector.  */
            logical_sector =  media_ptr -> fx_media_driver_logical_sector;
            count =  media_ptr -> fx_media_driver_sectors;
            while (count)
            {

                /* Call LevelX to release a sector mapping.  */
                status =  _lx_nand_flash_sector_release(p_nand_flash, logical_sector);

                /* Determine if the sector release was successful.  */
                if (status != LX_SUCCESS)
                {
                
                    /* Return an I/O error to FileX.  */
                    media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
                    
                    return;
                } 

                /* Successful sector release.  */
                count--;
                logical_sector++;
            }
          
            /* Successful driver request.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        case FX_DRIVER_FLUSH:
        {
            /* Return driver success.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        case FX_DRIVER_ABORT:
        {
            /* Return driver success.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        case FX_DRIVER_INIT:
        {
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        case FX_DRIVER_UNINIT:
        {
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }
        
        case FX_DRIVER_BOOT_READ:
        {
            p_nand_flash = (LX_NAND_FLASH *)media_ptr->fx_media_driver_info;
            
            /* Read the boot record and return to the caller.  */
            status =  _lx_nand_flash_sector_read(p_nand_flash, 0, (UCHAR *) media_ptr -> fx_media_driver_buffer);

            /* Determine if the read was successful.  */
            if (status != LX_SUCCESS)
            {
                
                /* Return an I/O error to FileX.  */
                media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
                    
                return;
            } 
            
            /* Successful driver request.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        case FX_DRIVER_BOOT_WRITE:
        {
            p_nand_flash = (LX_NAND_FLASH *)media_ptr->fx_media_driver_info;

            /* Write the boot record and return to the caller.  */
            status =  _lx_nand_flash_sector_write(p_nand_flash, 0, (UCHAR *) media_ptr -> fx_media_driver_buffer);

            /* Determine if the write was successful.  */
            if (status != LX_SUCCESS)
            {
                
                /* Return an I/O error to FileX.  */
                media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
                    
                return;
            } 

            /* Successful driver request.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break ;
        }

        default:
        {

            /* Invalid driver request.  */
            media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
            break;
        }
    }
}

