#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <exception>
#include "pdfwrapper.h"
#include "hpdf.h"

void error_handler (HPDF_STATUS   error_no,
               HPDF_STATUS   detail_no,
               void         *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n",
      (HPDF_UINT)error_no, (HPDF_UINT)detail_no);

    throw std::exception ();
}

pdfwrapper::pdfwrapper()
{
	
}
pdfwrapper::~pdfwrapper()
{
	
}

int pdfwrapper::Build( QString strFile, QString strTitle, QString strColTitle, QStringList body )
{
//    const char *page_title = strTitle.toLatin1().data();
    HPDF_Doc  pdf;
    HPDF_Page page;
    HPDF_Font def_font;
    HPDF_REAL tw;
    HPDF_REAL height;
    HPDF_REAL width;
    //HPDF_UINT i;

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        printf ("error: cannot create PdfDoc object\n");
        return 1;
    }
    try {
        /* Add a new page object. */
        page = HPDF_AddPage (pdf);

        height = HPDF_Page_GetHeight (page);
        width = HPDF_Page_GetWidth (page);

        /* Print the lines of the page. */
        HPDF_Page_SetLineWidth (page, 1);
        HPDF_Page_Rectangle (page, 50, 50, width - 100, height - 110);
        HPDF_Page_Stroke (page);

        /* Print the title of the page (with positioning center). */
        def_font = HPDF_GetFont (pdf, "Helvetica-Bold", NULL);
        HPDF_Page_SetFontAndSize (page, def_font, 20);

        tw = HPDF_Page_TextWidth(page, strTitle.toLatin1().data());
        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, (width - tw) / 2, height - 50);
        HPDF_Page_ShowText(page, strTitle.toLatin1().data());
        HPDF_Page_EndText(page);

        /* output subtitle. */
        HPDF_Page_BeginText(page);
//        HPDF_Page_MoveTextPos (page, 60, height - 80);
        tw = HPDF_Page_TextWidth(page, strColTitle.toLatin1().data());
        HPDF_Page_MoveTextPos(page, (width - tw) / 2, height - 80);
        HPDF_Page_SetFontAndSize (page, def_font, 13);
//        HPDF_Page_ShowText (page, "<Standerd Type1 fonts samples>");
        HPDF_Page_ShowText(page, strColTitle.toLatin1().data());
        HPDF_Page_EndText(page);

        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, 60, height - 105);

        //i = 0;
		
		for( int i=0; i < body.count(); i++ )
		{
            const char* text = body.at(i).toLatin1().data(); //  "abcdefgABCDEFG12345!#$%&+-@?";
//            HPDF_Font font = HPDF_GetFont (pdf, font_list[i], NULL);

            /* print a label of text */
            HPDF_Page_SetFontAndSize (page, def_font, 9);
            HPDF_Page_ShowText (page, text);
            HPDF_Page_MoveTextPos (page, 0, -18);

            /* print a sample text. */
//            HPDF_Page_SetFontAndSize (page, font, 20);
//            HPDF_Page_ShowText (page, samp_text);
                //HPDF_Page_MoveTextPos (page, 0, -20);
				
        }

        HPDF_Page_EndText( page );
		
        HPDF_SaveToFile( pdf, strFile.toLatin1().data() );
    } 
	catch (...) 
	{
        HPDF_Free (pdf);
        return 1;
    }

    /* clean up */
    HPDF_Free (pdf);

    return 0;
}
