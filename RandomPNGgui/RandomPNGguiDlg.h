
// RandomPNGguiDlg.h: file di intestazione
//

#pragma once


// Finestra di dialogo CRandomPNGguiDlg
class CRandomPNGguiDlg : public CDialogEx
{
// Costruzione
public:
	CRandomPNGguiDlg(CWnd* pParent = nullptr);	// costruttore standard

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RANDOMPNGGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// supporto DDX/DDV


// Implementazione
protected:
	HICON m_hIcon;
	CEdit *path,*width,*height,*text;
	CButton *transparency,*bit8,*bit16,*autogen,*textgen,*generate,*choosefile,*wordfirst,*wordlast,*endiandef,*endianbig,*endianlittle,*autoopen;
	CProgressCtrl* progress;

	void OnGenerateImageClick();
	void OnChooseFileClick();
	void OnTransparencyClick();
	void OnBitClick();
	void OnGenClick();
	void OnWordClick();
	void OnEndianDefClick();
	void OnEndianBigClick();
	void OnEndianLittleClick();
	void OnAutoOpenClick();


	// Funzioni generate per la mappa dei messaggi
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboboxex1();
};

#define	DEF_FILENAME		"image.png"
#define	DEF_WIDTH			"1920"
#define	DEF_HEIGHT			"1080"
#define	INTERLACE_TYPE		PNG_INTERLACE_NONE
#define	COMPRESSION_TYPE	PNG_COMPRESSION_TYPE_DEFAULT
#define	FILTER_TYPE			PNG_FILTER_TYPE_DEFAULT
#define	BASE_VALUE			10
#define	DEF_STRING			256

#define ERR_FILE			"Errore nella creazione del file "
#define	ERR_PNG				"Errore nell\'inizializzazione della libreria PNG"