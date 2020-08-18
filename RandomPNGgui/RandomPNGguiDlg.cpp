
// RandomPNGguiDlg.cpp: file di implementazione
//

#include "pch.h"
#include "framework.h"
#include "RandomPNGgui.h"
#include "RandomPNGguiDlg.h"
#include "afxdialogex.h"
#include <chrono>
#include <png.h>
#include <mpir.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Finestra di dialogo CRandomPNGguiDlg



CRandomPNGguiDlg::CRandomPNGguiDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RANDOMPNGGUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRandomPNGguiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRandomPNGguiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_GENERATE, OnGenerateImageClick)
	ON_BN_CLICKED(ID_CHOOSEFILE, OnChooseFileClick)
	ON_BN_CLICKED(ID_TRANSPARENCY, OnTransparencyClick)
	ON_BN_CLICKED(ID_8BIT, On8BitClick)
	ON_BN_CLICKED(ID_16BIT, On16BitClick)
	ON_BN_CLICKED(ID_AUTOGEN, OnAutoGenClick)
	ON_BN_CLICKED(ID_TEXTGEN, OnTextGenClick)
	//ON_CBN_SELCHANGE(IDC_COMBOBOXEX1, &CRandomPNGguiDlg::OnCbnSelchangeComboboxex1)
END_MESSAGE_MAP()


// Gestori di messaggi di CRandomPNGguiDlg

BOOL CRandomPNGguiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Aggiungere la voce di menu "Informazioni su..." al menu di sistema.

	// IDM_ABOUTBOX deve trovarsi tra i comandi di sistema.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Impostare l'icona per questa finestra di dialogo.  Il framework non esegue questa operazione automaticamente
	//  se la finestra principale dell'applicazione non è una finestra di dialogo.
	SetIcon(m_hIcon, TRUE);			// Impostare icona grande.
	SetIcon(m_hIcon, FALSE);		// Impostare icona piccola.

	// TODO: aggiungere qui inizializzazione aggiuntiva.
	HWND hwnd = AfxGetMainWnd()->GetSafeHwnd();
	generate = (CButton*)GetDlgItem(ID_GENERATE);
	choosefile = (CButton*)GetDlgItem(ID_CHOOSEFILE);
	path = (CEdit*)GetDlgItem(ID_PATH);
	width = (CEdit*)GetDlgItem(ID_WIDTH);
	height = (CEdit*)GetDlgItem(ID_HEIGHT);
	text = (CEdit*)GetDlgItem(ID_TEXT);
	transparency = (CButton*)GetDlgItem(ID_TRANSPARENCY);
	bit8 = (CButton*)GetDlgItem(ID_8BIT);
	bit16 = (CButton*)GetDlgItem(ID_16BIT);
	autogen = (CButton*)GetDlgItem(ID_AUTOGEN);
	textgen = (CButton*)GetDlgItem(ID_TEXTGEN);
	progress = (CProgressCtrl*)GetDlgItem(ID_PROGRESS);
	path->SetWindowTextW(_T(DEF_FILENAME));
	width->SetWindowTextW(_T(DEF_WIDTH));
	height->SetWindowTextW(_T(DEF_HEIGHT));
	bit8->SetCheck(BST_CHECKED);
	autogen->SetCheck(BST_CHECKED);
	text->EnableWindow(FALSE);
	return TRUE;  // restituisce TRUE a meno che non venga impostato lo stato attivo su un controllo.
}

void CRandomPNGguiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// Se si aggiunge alla finestra di dialogo un pulsante di riduzione a icona, per trascinare l'icona sarà necessario
//  il codice sottostante.  Per le applicazioni MFC che utilizzano il modello documento/visualizzazione,
//  questa operazione viene eseguita automaticamente dal framework.

void CRandomPNGguiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // contesto di dispositivo per il disegno

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrare l'icona nel rettangolo client.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Disegnare l'icona
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Il sistema chiama questa funzione per ottenere la visualizzazione del cursore durante il trascinamento
//  della finestra ridotta a icona.
HCURSOR CRandomPNGguiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRandomPNGguiDlg::OnCbnSelchangeComboboxex1()
{
	// TODO: aggiungere qui il codice del gestore di notifiche del controllo
}

void CRandomPNGguiDlg::OnGenerateImageClick()
{
	generate->EnableWindow(FALSE);
	choosefile->EnableWindow(FALSE);
	path->EnableWindow(FALSE);
	width->EnableWindow(FALSE);
	height->EnableWindow(FALSE);
	bit8->EnableWindow(FALSE);
	bit16->EnableWindow(FALSE);
	transparency->EnableWindow(FALSE);
	autogen->EnableWindow(FALSE);
	textgen->EnableWindow(FALSE);
	text->EnableWindow(FALSE);
	FILE* fp;
	LPTSTR file = new TCHAR[DEF_STRING],s = new TCHAR[DEF_STRING];
	path->GetWindowTextW(file,DEF_STRING);
	char* tmp = new char[DEF_STRING];
	_tfopen_s(&fp,file, _T("wb"));
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
				width->GetWindowTextW(s,DEF_STRING);
				size_t width = strtol(CT2CA(s), NULL, 10);
				height->GetWindowTextW(s, DEF_STRING);
				size_t height = strtol(CT2CA(s), NULL, 10);
				//printf("Generating %s, width: %d height: %d\n", argv[ARG_FILENAME], (int)width, (int)height);
				png_init_io(png_ptr, fp);
				int bit = bit8->GetCheck() == BST_CHECKED ? 8 : 16;
				int colour = transparency->GetCheck() == BST_CHECKED ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB;
				int pixelBits = (colour == PNG_COLOR_TYPE_RGB_ALPHA ? 4 : 3) * bit;
				png_set_IHDR(png_ptr, info_ptr, (png_uint_32)width, (png_uint_32)height, bit, colour, INTERLACE_TYPE, COMPRESSION_TYPE, FILTER_TYPE);
				png_write_info(png_ptr, info_ptr);

				mpz_t allImages, item;
				mpz_init(allImages);
				mpz_init(item);
				mpz_add_ui(allImages, allImages, 2);
				mpz_pow_ui(allImages, allImages, pixelBits);
				mpz_pow_ui(allImages, allImages, width * height);
				if (textgen->GetCheck() == BST_CHECKED)
				{
					text->GetWindowTextW(s, DEF_STRING);
					mpz_set_str(item, CT2CA(s), BASE_VALUE);
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
				size_t size = pixelBits / 8 * width;
				png_byte* pixels = new png_byte[size * height];
				png_byte* row = new png_byte[size];
				mpz_export(pixels, NULL, 1, bit / 8, 0, 0, item);
				mpz_clear(item);

				size_t i1, i2;
				progress->SetRange(0, height - 1);
				progress->SetPos(0);
				for (i1 = 0; i1 < height; i1++)
				{
					//printf("\nWriting row %d, %d%% of the image done", (int)(i1 + 1), (int)(i1 * 100 / height));
					for (i2 = 0; i2 < size; i2++)
					{
						row[i2] = pixels[i1 * size + i2];
					}
					png_write_row(png_ptr, row);
					progress->SetPos(i1);
				}


				delete[] row;
				delete[] pixels;
				png_write_end(png_ptr, info_ptr);
				//printf("\nGenerated image\n");
			}
			png_destroy_write_struct(&png_ptr, &info_ptr);
			fclose(fp);
			//system(file);
			ShellExecute(
				GetSafeHwnd(),
				_T("open"),
				file,
				NULL,
				NULL,
				SW_SHOWNORMAL
			);
		}
		else
		{
			fclose(fp);
			MessageBox(_T(ERR_PNG), NULL, MB_ICONERROR);
		}
	}
	else
	{
		snprintf(tmp, DEF_STRING, "%s %s", ERR_FILE, file);
		MessageBox(CA2W(tmp), NULL,MB_ICONERROR);
	}
	delete[] s;
	delete[] file;
	delete[] tmp;

	generate->EnableWindow(TRUE);
	choosefile->EnableWindow(TRUE);
	path->EnableWindow(TRUE);
	width->EnableWindow(TRUE);
	height->EnableWindow(TRUE);
	bit8->EnableWindow(TRUE);
	bit16->EnableWindow(TRUE);
	transparency->EnableWindow(TRUE);
	autogen->EnableWindow(TRUE);
	textgen->EnableWindow(TRUE);
	text->EnableWindow(TRUE);
}

void CRandomPNGguiDlg::OnChooseFileClick()
{
	TCHAR szFilters[] = _T("Immagine PNG (*.png)|*.png||");
	CFileDialog d(false, _T("*.png"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilters, AfxGetMainWnd(), 0, true);
	if (d.DoModal() == IDOK)
	{
		auto file = d.GetPathName();
		path->SetWindowTextW(file);
	}
}

void CRandomPNGguiDlg::OnTransparencyClick()
{
	//transparency->SetCheck(transparency->GetCheck() ? BST_UNCHECKED : BST_CHECKED);
}

void CRandomPNGguiDlg::On8BitClick()
{
	if (bit8->GetCheck() == BST_CHECKED)
	{
		bit8->SetCheck(BST_UNCHECKED);
		bit16->SetCheck(BST_CHECKED);
	}
	else
	{
		bit16->SetCheck(BST_UNCHECKED);
		bit8->SetCheck(BST_CHECKED);
	}
}

void CRandomPNGguiDlg::On16BitClick()
{
	if (bit16->GetCheck() == BST_CHECKED)
	{
		bit16->SetCheck(BST_UNCHECKED);
		bit8->SetCheck(BST_CHECKED);
	}
	else
	{
		bit8->SetCheck(BST_UNCHECKED);
		bit16->SetCheck(BST_CHECKED);
	}
}

void CRandomPNGguiDlg::OnAutoGenClick()
{
	if (autogen->GetCheck() == BST_CHECKED)
	{
		autogen->SetCheck(BST_UNCHECKED);
		textgen->SetCheck(BST_CHECKED);
		text->EnableWindow(TRUE);
	}
	else
	{
		textgen->SetCheck(BST_UNCHECKED);
		autogen->SetCheck(BST_CHECKED);
		text->EnableWindow(FALSE);
	}
}

void CRandomPNGguiDlg::OnTextGenClick()
{
	if (textgen->GetCheck() == BST_CHECKED)
	{
		textgen->SetCheck(BST_UNCHECKED);
		autogen->SetCheck(BST_CHECKED);
		text->EnableWindow(FALSE);
	}
	else
	{
		autogen->SetCheck(BST_UNCHECKED);
		textgen->SetCheck(BST_CHECKED);
		text->EnableWindow(TRUE);
	}
}