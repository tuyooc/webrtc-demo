#include <stdio.h>
#include <stdlib.h>

#include "speex_decode.h"

///*The frame size in hardcoded for this sample code but it doesn’t have to be*/
#define FRAME_SIZE 160


int  spx_encode_init(){
	speex_decode_union_t * speex_decode_u = malloc(sizeof(speex_decode_union_t));
    if(speex_decode_u == NULL){
        printf("speex_decode_union malloc failed ...\n");
        exit(1);
    }

    /*Create a new decoder state in narrowband mode*/
    speex_decode_u->state = speex_decoder_init(&speex_nb_mode);
	/*Set the perceptual enhancement on*/
    int tmp = 1;
    speex_decoder_ctl(speex_decode_u->state, SPEEX_SET_ENH, &tmp);

	/*Initialization of the structure that holds the bits*/
	speex_bits_init(&speex_decode_u->bits);

    return (int )speex_decode_u;
}


int spx_encode_frame(int handle ,char *src_speex_data ,short *target_pcm_data_buffer){
	speex_decode_union_t * speex_decode_u = (speex_decode_union_t *)handle;
//	/*Holds the audio that will be written to file (16 bits per sample)*/
//	short out[FRAME_SIZE];
	/*Speex handle samples as float, so we need an array of floats*/
	float output[FRAME_SIZE];

	int nbBytesSpeex = 38;
	/*Copy the data into the bit-stream struct*/  //speex_data -> receive speex audio data
	speex_bits_read_from(&speex_decode_u->bits, src_speex_data, nbBytesSpeex);
	/*Decode the data*/
	speex_decode(speex_decode_u->state, &speex_decode_u->bits, output); //here ,is float data

	/*Copy from float to short (16 bits) for output*/
	int i ;
	for (i = 0; i < FRAME_SIZE; i++)
		target_pcm_data_buffer[i] = output[i];

//	/*Write the decoded audio to file*/
//	fwrite(out, sizeof(short), FRAME_SIZE, fout);
	return 0;

}

int spx_destroy(int handle ){
	speex_decode_union_t * speex_decode_u = (speex_decode_union_t *)handle;

	/*Destroy the decoder state*/
	speex_decoder_destroy(speex_decode_u->state);
	/*Destroy the bit-stream truct*/
	speex_bits_destroy(&speex_decode_u->bits);
	free(speex_decode_u);
}


