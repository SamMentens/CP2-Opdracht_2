#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __DEBUG

#define BMPINPUTFILE "input.bmp"
#define MAXCHAR 1000

int main()
{
	// --------------------------------- START CODE ------------------------------------------

    FILE* inputFilePointer = fopen(BMPINPUTFILE, "rb"); //maak een file pointer naar de afbeelding
    if(inputFilePointer == NULL) //Test of het open van de file gelukt is!
    {
        printf("Something went wrong while trying to open %s\n", BMPINPUTFILE);
        exit(EXIT_FAILURE);
    }

     #ifdef __DEBUG
        printf("info: Opening File OK: %s\n", BMPINPUTFILE);
    #endif 

    unsigned char bmpHeader[54]; // voorzie een array van 54-bytes voor de BMP Header
    fread(bmpHeader, sizeof(unsigned char), 54, inputFilePointer); // lees de 54-byte header

    //Informatie uit de header (wikipedia)
    // haal de hoogte en breedte uit de header
    int breedte = *(int*)&bmpHeader[18];
    int hoogte = *(int*)&bmpHeader[22];

    #ifdef __DEBUG
        printf("info: breedte = %d\n", breedte);
        printf("info: hoogte = %d\n", hoogte);
    #endif
	
    int imageSize = 3 * breedte * hoogte; //ieder pixel heeft 3 byte data: rood, groen en blauw (RGB)
    unsigned char* inputPixels = (unsigned char *) calloc(imageSize, sizeof(unsigned char)); // allocate een array voor alle pixels
	
    fread(inputPixels, sizeof(unsigned char), imageSize, inputFilePointer); // Lees alle pixels (de rest van de file

    for(int i =0; i <= 3*8 -1; i+=3)
	{
		printf("pixel %d: B= %d, G=%d, R=%d\n", i/3+1, inputPixels[i], inputPixels[i+1], inputPixels[i+2]);
	}
	
	// --------------------------------- END START CODE --------------------------------------
	
	// ------------------------------------- FILTER ------------------------------------------
	
	unsigned char* inputPixelsB = (unsigned char *) calloc(imageSize, sizeof(unsigned char)); // allocate een array voor alle blauwe pixels
	unsigned char* inputPixelsG = (unsigned char *) calloc(imageSize, sizeof(unsigned char)); // allocate een array voor alle groene pixels
	unsigned char* inputPixelsR = (unsigned char *) calloc(imageSize, sizeof(unsigned char)); // allocate een array voor alle rood pixels
	
	for(int i = 0; i<imageSize ; i+=3)
	{
		if(inputPixels[i] >= inputPixels[i+1]) // Blauw >= Groen
		{
			if(inputPixels[i] >= inputPixels[i+2]) // Blauw >= Rood
			{
				inputPixelsB[i] = inputPixels[i];
				inputPixelsB[i+1] = inputPixels[i+1];
				inputPixelsB[i+2] = inputPixels[i+2];
			}
		}
		
		if(inputPixels[i+1] >= inputPixels[i]) // Groen >= Blauw
		{
			if(inputPixels[i+1] >= inputPixels[i+2]) // Groen >= Rood
			{
				inputPixelsG[i] = inputPixels[i];
				inputPixelsG[i+1] = inputPixels[i+1];
				inputPixelsG[i+2] = inputPixels[i+2];
			}
		}
		
		if(inputPixels[i+2] >= inputPixels[i]) // Rood >= Blauw
		{
			if(inputPixels[i+2] >= inputPixels[i+1]) // Rood >= Groen
			{
				inputPixelsR[i] = inputPixels[i];
				inputPixelsR[i+1] = inputPixels[i+1];
				inputPixelsR[i+2] = inputPixels[i+2];
			}
		}
		

	}
	
	printf("\n\n");
	
	for(int i =0; i <= 3*8 -1; i+=3)
	{
		printf("pixelB %d: B= %d, G=%d, R=%d\n", i/3+1, inputPixelsB[i], inputPixelsB[i+1], inputPixelsB[i+2]);
	}
	
	printf("\n\n");
	
	for(int i =0; i <= 3*8 -1; i+=3)
	{
		printf("pixelG %d: B= %d, G=%d, R=%d\n", i/3+1, inputPixelsG[i], inputPixelsG[i+1], inputPixelsG[i+2]);
	}
	
	printf("\n\n");
	
	
	for(int i =0; i <= 3*8 -1; i+=3)
	{
		printf("pixelR %d: B= %d, G=%d, R=%d\n", i/3+1, inputPixelsR[i], inputPixelsR[i+1], inputPixelsR[i+2]);
	}
	
	
	// ------------------------------------ END FILTER ----------------------------------------
	
	// ------------------------------ OUTPUT FILE GENERATOR -----------------------------------
	
	FILE* blueOut = fopen("blueOut.bmp","wb");
	FILE* greenOut = fopen("greenOut.bmp","wb");
	FILE* redOut = fopen("redOut.bmp","wb");
	
	for(int k=0;k<54;k++)
	{
		fputc(bmpHeader[k],blueOut);
	}
	
	for(int k=0;k<imageSize;k++)
	{
		fputc(inputPixelsB[k],blueOut);
	}
	
	for(int k=0;k<54;k++)
	{
		fputc(bmpHeader[k],greenOut);
	}
	
	for(int k=0;k<imageSize;k++)
	{
		fputc(inputPixelsG[k],greenOut);
	}
	
	for(int k=0;k<54;k++)
	{
		fputc(bmpHeader[k],redOut);
	}
	
	for(int k=0;k<imageSize;k++)
	{
		fputc(inputPixelsR[k],redOut);
	}
	
	// ------------------------------ END OUTPUT FILE GENERATOR ---------------------------------
	
	fclose(inputFilePointer);
	
	fclose(blueOut);
	fclose(greenOut);
	fclose(redOut);
	
    free(inputPixels);
    return 0;
}