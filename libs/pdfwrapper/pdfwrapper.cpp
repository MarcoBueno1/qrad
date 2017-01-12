#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <exception>
#include "pdfwrapper.h"
#include "hpdf.h"
#include "format.h"

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

void draw_line( HPDF_Page    page,
				float        x,
				float        y,
				const char  *label)
{
    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, x, y - 10);
    HPDF_Page_ShowText (page, label);
    HPDF_Page_EndText (page);

    HPDF_Page_MoveTo (page, x, y - 15);
    HPDF_Page_LineTo (page, x + 220, y - 15);
    HPDF_Page_Stroke (page);
}

int pdfwrapper::Build( QString strFile, 
                       QStringList ThreeLinesheader,
                       QString strTitle, 
					   QList<FieldFormat *> ColHeader, 
					   QList<QStringList *> lines )
{
    HPDF_Doc  pdf;
    HPDF_Page page;
    HPDF_Font def_font;
    HPDF_Font def_font_head;
    HPDF_Font def_font_title;
    HPDF_REAL tw;
    HPDF_REAL height;
    HPDF_REAL width;
    char szText[512];
    int i = 1;
    int iHeight = 1;
    int nPage = 0;

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) 
    {
        printf ("error: cannot create PdfDoc object\n");
        return 1;
    }
    try 
    {
        /* Add a new page object. */
        do 
        {
            nPage++;

            page = HPDF_AddPage (pdf);

            height = HPDF_Page_GetHeight (page);
            width = HPDF_Page_GetWidth (page);

            /* Print the lines of the page. */
            HPDF_Page_SetLineWidth (page, 1);
            HPDF_Page_Rectangle (page, 50, 50, width - 100, height - 110);
            HPDF_Page_Stroke (page);

            /// Tentativa de imprimir 3 linhas pequenas
            def_font_head = HPDF_GetFont (pdf, "Helvetica", "ISO8859-2");
			for( int j = 0; (j < ThreeLinesheader.count()) && (j < 3); j++)
			{
				HPDF_Page_SetFontAndSize (page, def_font_head, 8);
				HPDF_Page_BeginText(page);
				HPDF_Page_MoveTextPos(page, 50 , height - 30 -( j*10));
				Format::RemoveUnsuportedChar( szText, ThreeLinesheader.at(j).toUtf8().data() );
				HPDF_Page_ShowText(page, szText);
				HPDF_Page_EndText(page);

/*				HPDF_Page_BeginText(page);
				HPDF_Page_MoveTextPos(page, 50 , height - 40);
				
				Format::RemoveUnsuportedChar( szText,"Inovação ao seu Alcance" );
				HPDF_Page_ShowText(page, szText);
				HPDF_Page_EndText(page);
				
				HPDF_Page_BeginText(page);
				HPDF_Page_MoveTextPos(page, 50 , height - 50);
				Format::RemoveUnsuportedChar( szText,"R. Gastão Vidigal, 2700 São Paulo-SP");
				HPDF_Page_ShowText(page, szText);
				HPDF_Page_EndText(page);
*/
				///
			}
            HPDF_Page_BeginText(page);
            sprintf( szText, "Pagina: %d de %d", nPage, (int)lines.count()/35?lines.count()/35:1 );
            HPDF_Page_MoveTextPos(page, width-50-HPDF_Page_TextWidth(page, szText ) , height - 40);
            HPDF_Page_ShowText(page, szText);
            HPDF_Page_EndText(page);
			
            HPDF_Page_BeginText(page);
            sprintf( szText, "Tot. Itens: %d", (int)lines.count());
            HPDF_Page_MoveTextPos(page, width-50-HPDF_Page_TextWidth(page, szText ) , height - 50);
            HPDF_Page_ShowText(page, szText);
            HPDF_Page_EndText(page);

            HPDF_Page_BeginText(page);
			HPDF_Page_SetFontAndSize (page, def_font_head, 8);
            strcpy( szText, "Blink Sistemas - WatsApp: 92 98415-1066");
            HPDF_Page_MoveTextPos(page, 50 , 40);
            HPDF_Page_ShowText(page, szText);
            HPDF_Page_EndText(page);
			

            /* Print the title of the page (with positioning center). */
//            def_font = HPDF_GetFont (pdf, "Helvetica-Bold", "ISO8859-2");
            def_font_title = HPDF_GetFont (pdf, "Helvetica-Oblique", "ISO8859-2");
            HPDF_Page_SetFontAndSize (page, def_font_title, 20);

            def_font = HPDF_GetFont (pdf, "Helvetica", "ISO8859-2");
            tw = HPDF_Page_TextWidth(page, strTitle.toUtf8().data());
            HPDF_Page_BeginText(page);
            HPDF_Page_MoveTextPos(page, (width - tw) / 2, height - 50);
            Format::RemoveUnsuportedChar( szText, strTitle.toUtf8().data());
            HPDF_Page_ShowText(page, szText);
            HPDF_Page_EndText(page);


           /// header ....
	   Format::FormatColHead( page, 
				  def_font,
				  width -100, 
				  height - 80, 
				  ColHeader );
								  
//         HPDF_Page_SetLineWidth (page, 1);
//         HPDF_Page_LineTo(page, 55, width - 55 );
/*
           QString strHead =  Format::FormatColHead( ColHeader );

           HPDF_Page_BeginText(page);
           HPDF_Page_MoveTextPos (page, 60, height - 80);
           HPDF_Page_SetFontAndSize (page, def_font, 9);
           char szHeader[256];
           strcpy( szHeader, strHead.toLatin1().data());
           //HPDF_Page_MoveTextPos (page, 0, -18);
           HPDF_Page_ShowText (page, szHeader);
           HPDF_Page_EndText(page);
//           HPDF_Page_MoveTextPos (page, 0, -18);
*/
           
           HPDF_Page_BeginText(page);
           HPDF_Page_MoveTextPos(page, 60, height - 105);
           HPDF_Page_EndText( page );



           ///
           printf( "i 39=%d lines.count()=%d, i=%d \n", i%39, lines.count(), i );
           for(; (i % 39) && i <= lines.count(); i++ )
           {
//           HPDF_Page_BeginText(page);
//		   if( i == 1)
//			   HPDF_Page_MoveTextPos(page, 60, height - 105);
		   
  //             QString line = Format::FormatLine( ColHeader, lines.at(i) );
               Format::FormatLine(page, 
								  def_font,
								  width -100, 
//								  height - 80 -18*i,
                                  height - 80 -18*iHeight,
								  ColHeader, lines.at(i-1) );
								  
               iHeight++;
               if( iHeight == 39)
                   iHeight = 1;
//               HPDF_Page_MoveTextPos (page, 0, -18);
 
 /*
          //     HPDF_Page_BeginText(page);
               HPDF_Page_SetFontAndSize (page, def_font, 9);
               HPDF_Page_ShowText (page, line.toUtf8().data());
               //HPDF_Page_ShowText (page, line.toUtf8().data());
          //     HPDF_Page_EndText(page);
*/		  
//             HPDF_Page_EndText( page );
         }
           printf( "saiu do loop i=%d \n", i );
           i++;

//           HPDF_Page_EndText( page );

        }while( i <= lines.count());

		
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

#if(0)
/*
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
        HPDF_Page_SetFontAndSize (page, def_font, 9);
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
*/
#endif
