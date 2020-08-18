// RandomPNG.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <stdio.h>
#include <chrono>
#include <png.h>
#include <mpir.h>

#define	ARG_MINIMUM		4

#define	ARG_FILENAME	1
#define	ARG_WIDTH		2
#define	ARG_HEIGHT		3
#define	ARG_ITEM		4

#define	BIT_DEPTH				8
#define	COLOR_TYPE				PNG_COLOR_TYPE_RGB
#define	INTERLACE_TYPE			PNG_INTERLACE_NONE
#define	COMPRESSION_TYPE		PNG_COMPRESSION_TYPE_DEFAULT
#define	FILTER_TYPE				PNG_FILTER_TYPE_DEFAULT

#define	BASE_VALUE				62

#define	INFO_EXT				".txt"

#if		COLOR_TYPE == PNG_COLOR_TYPE_GRAY
#define	COLOR_CHANNELS			1
#elif	COLOR_TYPE == PNG_COLOR_TYPE_GRAY_ALPHA
#define	COLOR_CHANNELS			2
#elif	COLOR_TYPE == PNG_COLOR_TYPE_RGB
#define	COLOR_CHANNELS			3
#elif	COLOR_TYPE == PNG_COLOR_TYPE_RGB_ALPHA
#define	COLOR_CHANNELS			4
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define BUTTON_ID      1001

using namespace std;

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }

    hInst = hInstance; // Store instance handle in our global variable

    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 100,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Hello, World!");
    static HWND hButton;

    switch (message)
    {
    case WM_CREATE:

        hButton = CreateWindow(L"button", L"Label",
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            100, 10,
            50, 20,
            hWnd, (HMENU)BUTTON_ID,
            hInst, NULL);

        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Here your application is laid out.
        // For this introduction, we just print out "Hello, World!"
        // in the top left corner.
        TextOut(hdc,
            5, 5,
            greeting, _tcslen(greeting));
        // End application-specific layout section.

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}


int main(int argc,char** argv)
{
	if (argc >= ARG_MINIMUM)
	{
		FILE* fp;
		fopen_s(&fp, argv[ARG_FILENAME], "wb");
		if (fp)
		{
			png_structp png_ptr = png_create_write_struct
			(PNG_LIBPNG_VER_STRING, (png_voidp)NULL,
				NULL, NULL);
			if (png_ptr)
			{
				png_infop info_ptr = png_create_info_struct(png_ptr);
				if (info_ptr && !setjmp(png_jmpbuf(png_ptr)))
				{
					size_t width = strtol(argv[ARG_WIDTH], NULL, 10);
					size_t height = strtol(argv[ARG_HEIGHT], NULL, 10);
					printf("Generating %s, width: %d height: %d\n",argv[ARG_FILENAME],(int)width,(int)height);
					png_init_io(png_ptr, fp);
					png_set_IHDR(png_ptr, info_ptr,(png_uint_32) width,(png_uint_32) height, BIT_DEPTH, COLOR_TYPE, INTERLACE_TYPE, COMPRESSION_TYPE, FILTER_TYPE);
					png_write_info(png_ptr, info_ptr);

					mpz_t allImages,item;
					mpz_init(allImages);
					mpz_init(item);
					mpz_add_ui(allImages, allImages, 2);
					mpz_pow_ui(allImages, allImages, BIT_DEPTH * COLOR_CHANNELS);
					mpz_pow_ui(allImages, allImages, width * height);
					if (argc > ARG_MINIMUM)
					{
						mpz_set_str(item, argv[ARG_ITEM], BASE_VALUE);
					}
					else
					{
						gmp_randstate_t rand;
						gmp_randinit_mt(rand);
						gmp_randseed_ui(rand, static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
						mpz_urandomm(item, rand, allImages);
					}


					/*FILE* info;
					char* name = new char[strlen(argv[ARG_FILENAME]) + strlen(INFO_EXT) + 1];
					strcpy(name, argv[ARG_FILENAME]);
					fopen_s(&info, strcat(name, INFO_EXT), "wb");
					if (info)
					{
						fprintf(info, "Here it is the value %s has been made from (base %d):\n", argv[ARG_FILENAME], BASE_VALUE);
						mpz_out_str(info, BASE_VALUE, item);
						fprintf(info, "\n\nHere it is the total amount of possible images, given the resolution is %dx%d, the bit depth is %d and the colours channels count is %d:\n",width,height,BIT_DEPTH,COLOR_CHANNELS);
						mpz_out_str(info, 10, allImages);
						fprintf(info, "\n\nGood work!");
						fclose(info);
					}
					delete[] name;*/
					mpz_clear(allImages);
					size_t size = BIT_DEPTH * COLOR_CHANNELS / 8 * width;
					png_byte* pixels = new png_byte[size * height];
					png_byte* row = new png_byte[size];
					mpz_export(pixels, NULL, 1, BIT_DEPTH / 8, 0, 0, item);
					mpz_clear(item);

					size_t i1,i2;
					for (i1 = 0; i1 < height; i1++)
					{
						//printf("\nWriting row %d, %d%% of the image done", (int)(i1 + 1), (int)(i1 * 100 / height));
						for (i2 = 0; i2 < size; i2++)
						{
							row[i2] = pixels[i1 * size + i2];
						}
						png_write_row(png_ptr, row);
					}


					delete[] row;
					delete[] pixels;
					png_write_end(png_ptr, info_ptr);
					printf("\nGenerated image\n");
				}
				png_destroy_write_struct(&png_ptr, &info_ptr);

			}
			fclose(fp);
		}
	
	}
	else
		printf("Insufficent amount of arguments,\nyou must specify:\nFilename, width, height\n");
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
