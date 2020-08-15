// PNGtoText.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <mpir.h>

#define	ARG_MINIMUM		2

#define	ARG_FILENAME	1

/*
#define	BIT_DEPTH				16
#define	COLOR_TYPE				PNG_COLOR_TYPE_RGB_ALPHA
#define	INTERLACE_TYPE			PNG_INTERLACE_NONE
#define	COMPRESSION_TYPE		PNG_COMPRESSION_TYPE_DEFAULT
#define	FILTER_TYPE				PNG_FILTER_TYPE_DEFAULT

#if		COLOR_TYPE == PNG_COLOR_TYPE_GRAY
#define	COLOR_CHANNELS			1
#elif	COLOR_TYPE == PNG_COLOR_TYPE_GRAY_ALPHA
#define	COLOR_CHANNELS			2
#elif	COLOR_TYPE == PNG_COLOR_TYPE_RGB
#define	COLOR_CHANNELS			3
#elif	COLOR_TYPE == PNG_COLOR_TYPE_RGB_ALPHA
#define	COLOR_CHANNELS			4
#endif
*/

using namespace std;


int main(int argc, char** argv)
{
	if (argc >= ARG_MINIMUM)
	{
		FILE* fp;
		fopen_s(&fp, argv[ARG_FILENAME], "r");
		if (fp)
		{
			png_structp png_ptr = png_create_write_struct
			(PNG_LIBPNG_VER_STRING, (png_voidp)NULL,
				NULL, NULL);
			if (png_ptr)
			{
				png_infop info_ptr = png_create_info_struct(png_ptr);
				if (info_ptr)
				{
					if (!setjmp(png_jmpbuf(png_ptr)))
					{
						size_t width = strtol(argv[ARG_WIDTH], NULL, 10);
						size_t height = strtol(argv[ARG_HEIGHT], NULL, 10);
						printf("Generating %s, width: %d height: %d", argv[ARG_FILENAME], (int)width, (int)height);
						png_init_io(png_ptr, fp);
						png_set_IHDR(png_ptr, info_ptr, (png_uint_32)width, (png_uint_32)height, BIT_DEPTH, COLOR_TYPE, INTERLACE_TYPE, COMPRESSION_TYPE, FILTER_TYPE);
						png_write_info(png_ptr, info_ptr);

						mpz_t allImages, item;
						mpz_init(allImages);
						mpz_init(item);
						mpz_add_ui(allImages, allImages, 2);
						mpz_pow_ui(allImages, allImages, BIT_DEPTH * COLOR_CHANNELS);
						mpz_pow_ui(allImages, allImages, width * height);
						if (argc > ARG_MINIMUM)
						{
							mpz_set_str(item, argv[ARG_ITEM], 62);
						}
						else
						{
							gmp_randstate_t rand;
							gmp_randinit_mt(rand);
							gmp_randseed_ui(rand, static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
							mpz_urandomm(item, rand, allImages);

						}

						size_t size = BIT_DEPTH * COLOR_CHANNELS / 8 * width;
						png_byte* pixels = new png_byte[size * height];
						png_byte* row = new png_byte[size];
						mpz_export(pixels, NULL, 1, BIT_DEPTH / 8, 0, 0, item);

						size_t i1, i2;
						for (i1 = 0; i1 < height; i1++)
						{
							//printf("\n\nWriting row %d, %d%% of the image done", (int)(i1 + 1), (int)(i1 * 100 / height));
							for (i2 = 0; i2 < size; i2++)
							{
								row[i2] = pixels[i1 * size + i2];
							}
							png_write_row(png_ptr, row);
						}
						delete[] row;


						png_write_end(png_ptr, info_ptr);
						printf("\nGenerated image\n");
					}
					png_destroy_write_struct(&png_ptr, &info_ptr);
				}
				else
				{
					png_destroy_write_struct(&png_ptr,
						(png_infopp)NULL);
				}

			}
			fclose(fp);
		}

	}
	else
		printf("Insufficent amount of arguments,\nyou must specify:\nFilename\n");
}




// Per eseguire il programma: CTRL+F5 oppure Debug > Avvia senza eseguire debug
// Per eseguire il debug del programma: F5 oppure Debug > Avvia debug

// Suggerimenti per iniziare: 
//   1. Usare la finestra Esplora soluzioni per aggiungere/gestire i file
//   2. Usare la finestra Team Explorer per connettersi al controllo del codice sorgente
//   3. Usare la finestra di output per visualizzare l'output di compilazione e altri messaggi
//   4. Usare la finestra Elenco errori per visualizzare gli errori
//   5. Passare a Progetto > Aggiungi nuovo elemento per creare nuovi file di codice oppure a Progetto > Aggiungi elemento esistente per aggiungere file di codice esistenti al progetto
//   6. Per aprire di nuovo questo progetto in futuro, passare a File > Apri > Progetto e selezionare il file con estensione sln
