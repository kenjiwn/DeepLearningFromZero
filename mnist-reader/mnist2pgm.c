#include <stdio.h>
#include <stdlib.h>

#define IM_W 20
#define IM_H 20

// convert MNIST training image file to PNM file
// first 20x20 images are output.
int main()
{
  // MNIST training file can be downloaded from http://yann.lecun.com/exdb/mnist/
  char fileName[] = "train-images.idx3-ubyte";

  // open MNIST image file
  FILE *fp = fopen( fileName, "rb" );
  if( fp == NULL ){
    fprintf( stderr, "ERROR: Cannot open %s.\n", fileName );
    fprintf( stderr, "MNIST data files can be downloaded from http://yann.lecun.com/exdb/mnist/\n" );
    return 1;
  }

  // read magic number 0x0000
  char magic[2];
  int sz = fread( magic, 1, 2, fp );
  if( sz != 2 ){
    fprintf( stderr, "ERROR: read error %s.\n", fileName );
    return 1;
  }
  if( magic[0] != 0 || magic[1] != 0 ){
    fprintf( stderr, "ERROR: file format error %s.\n", fileName );
    return 1;
  }
  fprintf( stderr, "INFO: magic number=0x0000.\n" );

  // read datatype
  char datatype;
  sz = fread( &datatype, 1, 1, fp );
  if( sz != 1 ){
    fprintf( stderr, "ERROR: read error %s.\n", fileName );
    return 1;
  }
  if( datatype != 0x08 ){
    fprintf( stderr, "ERROR: file format error %s.\n", fileName );
    return 1;
  }
  fprintf( stderr, "INFO: datatype=%d.\n", (int)datatype );


  // read dimension
  char dim;
  sz = fread( &dim, 1, 1, fp );
  if( sz != 1 ){
    fprintf( stderr, "ERROR: read error %s.\n", fileName );
    return 1;
  }
  if( dim != 0x03 ){
    fprintf( stderr, "ERROR: file format error %s.\n", fileName );
    return 1;
  }
  fprintf( stderr, "INFO: dimension=%d.\n", (int)dim );

  // read size in dimensions
  char dims[4];
  int N[3];
  for( int i=0; i<3; i++ ){
    sz = fread( dims, 1, 4, fp );
    if( sz != 4 ){
      fprintf( stderr, "ERROR: read error %s.\n", fileName );
      return 1;
    }
    N[i] = 0;
    for( int j=0; j<4; j++ ){
      N[i] = (N[i] << 8) | (dims[j] & 0xff);
    }
    fprintf( stderr, "INFO: dimension %d length=%d.\n", i, N[i] );
  }

  // read data
  int totalLength = N[0] * N[1] * N[2];
  char *data = (char *) malloc( totalLength);
  if( data == NULL ){
      fprintf( stderr, "ERROR: malloc.\n" );
      return 1;
  }
  sz = fread( data, 1, totalLength, fp );
  if( sz != totalLength ){
    fprintf( stderr, "ERROR: read error %s.\n", fileName );
    return 1;
  }

  // open PGM image file
  char fileNameOut[] = "output.pgm";
  FILE *fpw = fopen( fileNameOut, "wb" );
  if( fpw == NULL ){
    fprintf( stderr, "ERROR: Cannot open %s.\n", fileNameOut );
  }
  
  // output pgm
  fprintf( fpw, "P2\n" );
  fprintf( fpw, "%d %d\n", IM_H * N[1], IM_W * N[2] );
  fprintf( fpw, "255\n" );
  for( int imY = 0; imY < IM_H; imY++ ){
    for( int y = 0; y < N[1]; y++ ){
      for( int imX = 0; imX < IM_W; imX++ ){
        for( int x = 0; x < N[2]; x++ ){
          int imIdx = imY * IM_W + imX;
          int pos = imIdx * N[1] * N[2] + y * N[2] + x;
          int pix = 255 - data[pos];
          fprintf( fpw, "%4d", pix );
        }
      }
    }
  }
  fclose( fpw );
  fclose( fp );

  fprintf( stderr, "INFO: output PGM file %s success!!!\n", fileNameOut );
}
