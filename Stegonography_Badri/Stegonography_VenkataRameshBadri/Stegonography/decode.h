#ifndef DECODE_H
#define DECODE_H
#include "types.h"
#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4
typedef struct _DecodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char *magic_data;
    char *file_extn;
    char *output_fname;
    FILE *fptr_output_data;
    char *secret_file_data;
}DecodeInfo;

/* Decoding function prototypes */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Open files for decoding */
Status open_decode_files(DecodeInfo *decInfo);

/* Decode magic string size */
Status decode_magic_string_size(DecodeInfo *decInfo);

/* Decode image to data */
Status decode_image_to_data(int size, FILE *fptr_stego_image, DecodeInfo *decInfo);

/* Decode from lsb lyte */
Status decode_from_lsb_byte(char *data,char *buffer,DecodeInfo *decInfo);

/* Decode magic string */
Status decode_magic_string(int magic_string_size,DecodeInfo *decInfo);

/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file extension */
Status decode_secret_file_extn(char secret_file_size,DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Secode secret file data */
Status decode_secret_file_data(int secret_file_size,DecodeInfo *decInfo);




#endif
