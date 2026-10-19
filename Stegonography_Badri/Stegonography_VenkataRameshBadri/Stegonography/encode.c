#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"




/* Function Definitions */


/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
char magic_string[20];
char file_extn[20];

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
//function definition
Status read_and_validate_encode_args( char *argv[], EncodeInfo *encInfo )
{
    //checking argument 2 is .bmp file are not
    if (( argv[2] != NULL ) && (strcmp((strstr(argv[2], ".")),".bmp") == 0))
    {
	//assigning the . argument 2 to the source image
	encInfo -> src_image_fname = argv[2];
    }
    else
    {
	// if not return failure
	return e_failure;
    }
    //checking argument 3 is .txt or .sh or .c file are not
    if((strcmp(strstr(argv[3],"."),".txt") == 0) || (strcmp(strstr(argv[3],"."),".sh") == 0) || (strcmp(strstr(argv[3],"."),".c") == 0))
    {
	//assigning the .txt/.c/.sh argument 3 to the secret fname
	encInfo -> secret_fname = argv[3];
	//copying to the file extension . to argument 3
	strcpy(file_extn,strchr(argv[3], '.'));
    }
     else
    {
	//if not return failure
	return e_failure;
    }
    //checking argument 4 is null or not
    if( argv[4] == NULL )
    {
	//assigning the stego.bmp to the stego image
	encInfo -> stego_image_fname = "stego.bmp";
    }
    else
    {
	//if not checking argument 4 with .bmp or not
	if(strstr(argv[4], ".bmp"))
	{
	    //assigning the argument 4 to stego image
	    encInfo -> stego_image_fname = argv[4];
	}
	else
	{
	    //print its not a bmp file
	    return e_failure;
	}
    }
    //prints successful
    return e_success;
}
Status encode_magic_string(char *magic_string,EncodeInfo *encInfo)
{
	//calling the function
    encode_data_to_image(magic_string,strlen(magic_string),encInfo -> fptr_src_image,encInfo -> fptr_stego_image,encInfo);
    //returning the success
    return e_success;
}
Status encode_magic_string_size(int size, EncodeInfo *encInfo)
{
    //declaring the character array
    char str[32];
    //reading the source image
    fread(str,32 , 1,encInfo -> fptr_src_image);
    //calling the function
    encode_size_to_lsb(str,size);
    //writing the stego image
    fwrite(str, 32, 1,encInfo -> fptr_stego_image);
    //returning the success
    return e_success;
}
Status encode_data_to_image( char *data,int size,FILE *fptr_src_image,FILE *fptr_stego_image,EncodeInfo *encInfo)
{
	for(int i=0;i<size;i++)
	{
	    //read the souce image
  	fread(encInfo -> image_data,8,1,fptr_src_image);
	//calling the function
	encode_byte_to_lsb(data[i], encInfo -> image_data);
	//writing the stego image 
 	fwrite(encInfo -> image_data,8,1,fptr_stego_image);
	}
	return e_success;


}
Status encode_secret_file_size(int file_size,EncodeInfo *encInfo)
{
	    //declaring the character array
    char str[32];
    //read the source image
    fread(str, 32, 1, encInfo -> fptr_src_image);
    //calling the function
    encode_size_to_lsb(str,file_size);
    //write the stego image
    fwrite(str, 32, 1, encInfo -> fptr_stego_image);
    return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
	//declaring the character array
    char str[encInfo -> size_secret_file];
    //moving the file pointer to 0 position
    fseek(encInfo -> fptr_secret, 0, SEEK_SET);
    //read the secret file 
    fread(str, encInfo -> size_secret_file,1,encInfo -> fptr_secret);
    //calling the function
    encode_data_to_image(str,encInfo -> size_secret_file, encInfo -> fptr_src_image, encInfo -> fptr_stego_image, encInfo);
    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src,FILE *fptr_dest)
{
	char ch;
	while(fread(&ch,1,1,fptr_src)>0)
	{
	fwrite(&ch,1,1,fptr_dest);
	}
	return e_success;
		
}
Status encode_secret_file_extn(char *file_extn,EncodeInfo *encInfo)
{
	encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image,encInfo);
	

}

Status encode_byte_to_lsb(char data,char *image_buffer)
{
	
    //declaring the integer i
    int i;
    //declaring the ret variable
    uint ret;
    // initialising the unsigned bit has 7 bits
    uint bit = 7;
    //loop runs until 8
    for(i=0;i<8;i++)
    {
	//encoding the byte to lsb
	image_buffer[i] = (image_buffer[i] & ~(1));
	ret = ((1<<(bit-i) & data) >> (bit-i));
	image_buffer[i] = image_buffer[i] | ret;
    }
}
Status encode_size(int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    //declaring the character array
    char str[32];
    //read the source image
    fread(str, 32, 1, fptr_src_image);
    //calling the function
    encode_size_to_lsb(str,size);
    //write the stego image
    fwrite(str, 32, 1, fptr_stego_image);
    return e_success;
}
Status encode_size_to_lsb(char *buf,int size)
{
	//declaring the integer i 
    int i;
    //declaring unsigned ret variable
    uint ret;
    //initialising the unsigned bit has 31
    uint bit=31;
    //loop runs until 32
    for(i=0;i<32;i++)
    {
	//encode size to lsb
	buf[i] = (buf[i] & ~(1));
	ret = ((1<<(bit-i) & size) >> (bit-i));
	buf[i] = buf[i] | ret;
    }
}

Status check_capacity(EncodeInfo *encInfo)
{
	encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
	encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
	 printf("Magic string encoded successfully\n");
         //we are storing string string length in magic string size variable
         uint magic_string_size=strlen(magic_string);
         // initialising the 4 for thr ext size variable
         uint ext_size = 4;
         //initialising the file extension length in ext variable
         uint ext = strlen(file_extn);
         uint secret_data_size = get_file_size(encInfo -> fptr_secret) - 1;
         uint ret = 54 + ((encInfo -> size_secret_file) + (magic_string_size) + (ext_size) + (ext) + (secret_data_size));
        //in ret we are storing the ret * 8
        ret = ret * 8;
        //checking condition
        if((encInfo -> image_capacity) > ret)
        {
	//condition true return success
	return e_success;
        }  
        else
        {
	//condition false then it will return failure
	return e_failure;
        }
		
}
uint get_file_size(FILE *fptr)
{
	fseek(fptr, 0, SEEK_END);
	return ftell(fptr);
}
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_dest_image)
{
	char str[54];
	fseek(fptr_src_image, 0,SEEK_SET);
	fread(str,54,1,fptr_src_image);
	fwrite(str,54,1,fptr_dest_image);
	return e_success;
}

//function definition
Status do_encoding(EncodeInfo *encInfo)
{
    //if open files is equal to success
    if( open_files(encInfo) == e_success )
    {
	//prints the open files successfull
	printf("Open file is Successful\n");
    }
    else
    {
	printf("Open file failed\n");
	return e_failure;
    }
    //checking the check capacity equal to success or not
    if(check_capacity(encInfo) == e_success)
    {
	//prints check capacity is successfull as output
	printf("Check capacity is successful\n");
	//checking the copy bmp header is equal to success or not
	if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success )
	{
	    //prints copy bmp header is successfull as output
	    printf("Successfully copied BMP header\n");
	    //checking the magic string size is equal to success or not
	    if(encode_magic_string_size(strlen(magic_string), encInfo) == e_success)
	    {
		//prints magic string size is successfull as output
		printf("Encoded magic string size is successful\n");
		//checking the magic string is equal to success or not
		if(encode_magic_string(magic_string, encInfo) == e_success )
		{
		    //prints magic string is successfull as output
		    printf("Encoded magic String is successfully encoded\n");
		    //checking encode size is equal to success or not
		    if( encode_size(strlen(strstr(encInfo -> secret_fname, ".")),encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success )
		    {
			//prints secret file extension size is successfull as output
		       	printf("Encoded secret file extension size is successful\n");
			//checking the secret file extension is equal to success or not
		       	if( encode_secret_file_extn(strstr(encInfo -> secret_fname, "."), encInfo) == e_success)
		       	{
			    //prints secret file extension is successfull as output
			    printf("Encoded secret file extension is successful\n");
			    //checking the secret file size is equal to success or not
			    if(encode_secret_file_size(encInfo -> size_secret_file, encInfo ) == e_success)
			    {
				//prints as output
			       	printf("Encoded secret file size is successful\n");
				//checking secret file data is equal to success or not
			       	if(encode_secret_file_data(encInfo) == e_success )
			       	{
				    //prints as output
				    printf("Encoded secret file data is successful\n");
				    //checking copy remaining data is equal to success or not
				    if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
				    {
					//prints as output
				       	printf("Copy remaining data is successful\n");
				    }
				    else
				    {
					//prints as output
				       	printf("Copy remaining data is failure\n");
					//condition false then it will return failure
				       	return e_failure;
				    }
			       	}
			       	else
			       	{
				    //prints as output
				    printf("Secret file data is failure\n");
				    //condition false then it will return failure
				    return e_failure;
			       	}
			    }
			    else
			    {
				//prints as output
			       	printf("Secret file size was failure\n");
				//condition false then it will return failure
			       	return e_failure;
			    }
		       	}
		       	else
		       	{
			    //prints as output
			    printf("Secret file extensionn was failure\n");
			    //condition false then it will return failure
			    return e_failure;
		       	}
		    }
		    else
		    {
			//prints as output
		       	printf("Secret file extension size was failure\n");
			//condition false then it will return failure
		       	return e_failure;
		    }
	       	}
	       	else
	       	{
		    //prints as output
		    printf("Magic string was failure\n");
		    //condition false then it will return failure
		    return e_failure;
	       	}
	    }
	    else
	    {
		//prints as output
		printf("Magic string size is failure\n");
		//condition false then it will return failure
		return e_failure;
	    }
	}
	else
	{
	    //prints as output
	    printf("Copy bmp header was failure\n");
	    //condition false then it will return failure
	    return e_failure; 
	}
    }
    else
    {
	//prints as output
	printf("Check Capacity was failure\n");
	//condition false then it will return failure
	return e_failure;
    }
    //all conditions true then it will return success
    return e_success;
}
				
