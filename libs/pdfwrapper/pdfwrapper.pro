#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T14:19:01
#
#-------------------------------------------------

QT       += core sql

#QT       -= gui

TARGET = pdfwrapper 

TEMPLATE = lib

INCLUDEPATH       += ../3rdpart/libharu/src/include
#win32:INCLUDEPATH += ../3rdpart/libharu/src/win32/include
INCLUDEPATH  +=  ../3rdpart/libharu/bin/include/

SOURCES += pdfwrapper.cpp  \
           format.cpp \
    ../3rdpart/libharu/src/src/hpdf_3dmeasure.c \
    ../3rdpart/libharu/src/src/hpdf_annotation.c \
    ../3rdpart/libharu/src/src/hpdf_array.c \
    ../3rdpart/libharu/src/src/hpdf_binary.c \
    ../3rdpart/libharu/src/src/hpdf_boolean.c \
    ../3rdpart/libharu/src/src/hpdf_catalog.c \
    ../3rdpart/libharu/src/src/hpdf_destination.c \
    ../3rdpart/libharu/src/src/hpdf_dict.c \
    ../3rdpart/libharu/src/src/hpdf_doc.c \
    ../3rdpart/libharu/src/src/hpdf_doc_png.c \
    ../3rdpart/libharu/src/src/hpdf_encoder.c \
    ../3rdpart/libharu/src/src/hpdf_encoder_cns.c \
    ../3rdpart/libharu/src/src/hpdf_encoder_cnt.c \
    ../3rdpart/libharu/src/src/hpdf_encoder_jp.c \
    ../3rdpart/libharu/src/src/hpdf_encoder_kr.c \
    ../3rdpart/libharu/src/src/hpdf_encoder_utf.c \
    ../3rdpart/libharu/src/src/hpdf_encrypt.c \
    ../3rdpart/libharu/src/src/hpdf_encryptdict.c \
    ../3rdpart/libharu/src/src/hpdf_error.c \
    ../3rdpart/libharu/src/src/hpdf_exdata.c \
    ../3rdpart/libharu/src/src/hpdf_ext_gstate.c \
    ../3rdpart/libharu/src/src/hpdf_font.c \
    ../3rdpart/libharu/src/src/hpdf_font_cid.c \
    ../3rdpart/libharu/src/src/hpdf_font_tt.c \
    ../3rdpart/libharu/src/src/hpdf_font_type1.c \
    ../3rdpart/libharu/src/src/hpdf_fontdef.c \
    ../3rdpart/libharu/src/src/hpdf_fontdef_base14.c \
    ../3rdpart/libharu/src/src/hpdf_fontdef_cid.c \
    ../3rdpart/libharu/src/src/hpdf_fontdef_cns.c \
    ../3rdpart/libharu/src/src/hpdf_fontdef_cnt.c \
    ../3rdpart/libharu/src/src/hpdf_fontdef_jp.c \
    ../3rdpart/libharu/src/src/hpdf_fontdef_kr.c \
    ../3rdpart/libharu/src/src/hpdf_fontdef_tt.c \
    ../3rdpart/libharu/src/src/hpdf_fontdef_type1.c \
    ../3rdpart/libharu/src/src/hpdf_gstate.c \
    ../3rdpart/libharu/src/src/hpdf_image.c \
    ../3rdpart/libharu/src/src/hpdf_image_ccitt.c \
    ../3rdpart/libharu/src/src/hpdf_image_png.c \
    ../3rdpart/libharu/src/src/hpdf_info.c \
    ../3rdpart/libharu/src/src/hpdf_list.c \
    ../3rdpart/libharu/src/src/hpdf_mmgr.c \
    ../3rdpart/libharu/src/src/hpdf_name.c \
    ../3rdpart/libharu/src/src/hpdf_namedict.c \
    ../3rdpart/libharu/src/src/hpdf_null.c \
    ../3rdpart/libharu/src/src/hpdf_number.c \
    ../3rdpart/libharu/src/src/hpdf_objects.c \
    ../3rdpart/libharu/src/src/hpdf_outline.c \
    ../3rdpart/libharu/src/src/hpdf_page_label.c \
    ../3rdpart/libharu/src/src/hpdf_page_operator.c \
    ../3rdpart/libharu/src/src/hpdf_pages.c \
    ../3rdpart/libharu/src/src/hpdf_pdfa.c \
    ../3rdpart/libharu/src/src/hpdf_real.c \
    ../3rdpart/libharu/src/src/hpdf_streams.c \
    ../3rdpart/libharu/src/src/hpdf_string.c \
    ../3rdpart/libharu/src/src/hpdf_u3d.c \
    ../3rdpart/libharu/src/src/hpdf_utils.c \
    ../3rdpart/libharu/src/src/hpdf_xref.c

HEADERS += pdfwrapper.h  \
           format.h \
    ../3rdpart/libharu/src/src/t4.h \
    ../3rdpart/libharu/src/include/hpdf.h \
    ../3rdpart/libharu/src/include/hpdf_3dmeasure.h \
    ../3rdpart/libharu/src/include/hpdf_annotation.h \
    ../3rdpart/libharu/src/include/hpdf_catalog.h \
    ../3rdpart/libharu/src/include/hpdf_conf.h \
    ../3rdpart/libharu/src/include/hpdf_consts.h \
    ../3rdpart/libharu/src/include/hpdf_destination.h \
    ../3rdpart/libharu/src/include/hpdf_doc.h \
    ../3rdpart/libharu/src/include/hpdf_encoder.h \
    ../3rdpart/libharu/src/include/hpdf_encrypt.h \
    ../3rdpart/libharu/src/include/hpdf_encryptdict.h \
    ../3rdpart/libharu/src/include/hpdf_error.h \
    ../3rdpart/libharu/src/include/hpdf_exdata.h \
    ../3rdpart/libharu/src/include/hpdf_ext_gstate.h \
    ../3rdpart/libharu/src/include/hpdf_font.h \
    ../3rdpart/libharu/src/include/hpdf_fontdef.h \
    ../3rdpart/libharu/src/include/hpdf_gstate.h \
    ../3rdpart/libharu/src/include/hpdf_image.h \
    ../3rdpart/libharu/src/include/hpdf_info.h \
    ../3rdpart/libharu/src/include/hpdf_list.h \
    ../3rdpart/libharu/src/include/hpdf_mmgr.h \
    ../3rdpart/libharu/src/include/hpdf_namedict.h \
    ../3rdpart/libharu/src/include/hpdf_objects.h \
    ../3rdpart/libharu/src/include/hpdf_outline.h \
    ../3rdpart/libharu/src/include/hpdf_page_label.h \
    ../3rdpart/libharu/src/include/hpdf_pages.h \
    ../3rdpart/libharu/src/include/hpdf_pdfa.h \
    ../3rdpart/libharu/src/include/hpdf_streams.h \
    ../3rdpart/libharu/src/include/hpdf_types.h \
    ../3rdpart/libharu/src/include/hpdf_u3d.h \
    ../3rdpart/libharu/src/include/hpdf_utils.h \
    ../3rdpart/libharu/src/include/hpdf_version.h
		   


target.path = $${PREFIX}/bin
INSTALLS += target

