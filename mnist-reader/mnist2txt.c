#include <stdio.h>
#include <stdlib.h>

#define IM_W 20
#define IM_H 20

// convert MNIST training label file to Text
// first 20x20 labels are output.
int main()
{
  // MNIST training file can be downloaded from http://yann.lecun.com/exdb/mnist/
  char fileName[] = "train-labels.idx1-ubyte";

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
  if( dim != 0x01 ){
    fprintf( stderr, "ERROR: file format error %s.\n", fileName );
    return 1;
  }
  fprintf( stderr, "INFO: dimension=%d.\n", (int)dim );

  // read size in dimensions
  char dims[4];
  int N[1];
  for( int i=0; i<1; i++ ){
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
  int totalLength = N[0];
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

  // output Text
  for( int imY = 0; imY < IM_H; imY++ ){
    for( int imX = 0; imX < IM_W; imX++ ){
      int imIdx = imY * IM_W + imX;
      printf( "%c", '0' + data[imIdx] );
    }
    printf( "\n" );
  }
  fclose( fp );
}
