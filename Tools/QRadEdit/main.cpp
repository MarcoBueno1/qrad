#include <QApplication>
#include <QCryptographicHash>
#include "mainwindow.h"

#include "qradsecure.h"
#include "qradsecurefile.h"

#define ACTION_ENCRYPT  0
#define ACTION_DECRYPT  1

bool simpleEncryption(char *file, char *outFile);
bool decrypt(char *inFile, char *outFile);
bool encrypt(char *inFile, char *outFile);
int  parseArgs(int argc, char *argv[]);
void printUsage();

bool (*EncryptFunc)(char*, char*) = encrypt;
bool (*DecryptFunc)(char*, char*) = decrypt;

int main(int argc, char *argv[])
{
    if (argc > 4)
        return parseArgs(argc, argv);
    else if ((argc > 2) && (argc <= 4))
    {
        printUsage();
        return -1;
    }
    else if (argc > 1)
    {
        if (('-' == argv[1][0]) && ('h' == argv[1][1]))
        {
            printUsage();
            return -1;
        }
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags( Qt::WindowTitleHint |  Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint
                      | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    w.showMaximized();

//    w.show();

    return a.exec();
}

int parseArgs(int argc, char *argv[])
{
    char inFile[512];
    char outFile[512];
    char action = -1;

    memset(inFile, 0, sizeof(inFile));
    memset(outFile, 0, sizeof(outFile));

    for (int i = 0; i < argc; i++)
    {
        if (argv[i][0] != '-')
            continue;

        switch(argv[i][1])
        {
            case 's':
            {
                EncryptFunc = simpleEncryption;
                DecryptFunc = simpleEncryption;
            }
            break;

            case 'e':
            {
                if ((i+1) >= argc)
                {
                    printUsage();
                    return -1;
                }

                strncpy(inFile, argv[i+1], sizeof(inFile));
                action = ACTION_ENCRYPT;
            }break;

            case 'd':
            {
                if ((i+1) >= argc)
                {
                    printUsage();
                    return -1;
                }

                strncpy(inFile, argv[i+1], sizeof(inFile));
                action = ACTION_DECRYPT;
            }
            break;

            case 'o':
            {
                if ((i+1) >= argc)
                {
                    printUsage();
                    return -1;
                }

                strncpy(outFile, argv[i+1], sizeof(outFile));
            }
            break;

            default:
            {
                printUsage();
                return -1;
            }
        } // switch
    } // for

    if ((action < 0) || (strlen(inFile) == 0) || (strlen(outFile) == 0))
    {
        printUsage();
        return -1;
    }
    
    ///
    /// @mbueno - here check password ...
    ///
//    LNX_CHECK_PASSWORD();

    if (ACTION_ENCRYPT == action)
        EncryptFunc(inFile, outFile);
    else
        DecryptFunc(inFile, outFile);

    return 0;
}

void printUsage()
{
    fprintf(stderr, "\nUsage: m2edit [[-s] (-e|-d) <input file> -o <output file>] password \n\n"
                    "\t-s  simple encrypt/decrypt <input file> and write result in <output file>.\n"
                    "\t-e  encrypt <input file> and write encrypted content in <output file>.\n"
                    "\t-d  decrypt <input file> and write decrypted content in <output file>.\n"
                    "\tpassword  Your knowed password to use this tool.\n"
                    "\t-h  this text.\n\n");
}

bool simpleEncryption(char *inFile, char *outFile)
{
    FILE *in, *out;

    in = fopen(inFile, "rb");

    if (NULL == in)
    {
        fprintf(stderr, "Could not open file: %s\n", inFile);
        return false;
    }

    out = fopen(outFile, "wb");

    if (NULL == out)
    {
        fprintf(stderr, "Could not create file: %s\n", outFile);
        fclose(in);
        return false;
    }

    int bytesRead = 0;
    char buffer[512];

    while (!feof(in))
    {
        bytesRead = fread(buffer, 1, sizeof(buffer), in);

        if (bytesRead > 0)
        {
            int i;

            for (i = 0; i < bytesRead; i++)
                buffer[i] = (buffer[i] ^ 0x9a);

            fwrite(buffer, 1, bytesRead, out);
        }
    } // while

    fclose(in);
    fclose(out);

    return true;
}

bool decrypt(char *inFile, char *outFile)
{
    QFile out(QString::fromUtf8(outFile));
    QRadSecureFile in(QString::fromUtf8(inFile));

    if (!in.open(QFile::ReadOnly))
    {
        fprintf(stderr, "Could not open file: %s\n", inFile);
        return false;
    }

    if (!out.open(QFile::WriteOnly))
    {
        fprintf(stderr, "could not open file: %s\n", outFile);
        in.close();
        return false;
    }

    QByteArray decryptedData = in.readAll();

    if (out.write(decryptedData) != decryptedData.length())
    {
        fprintf(stderr,"Failed to write decrypted file.\n");
    }
    else
    {
        fprintf(stdout, "Decrypted content written to file: %s\n\n", outFile);
    }

    out.close();
    in.close();

    return true;
}

bool encrypt(char *inFile, char *outFile)
{
    QRadSecureFile out(QString::fromUtf8(outFile));
    QFile in(QString::fromUtf8(inFile));

    if (!in.open(QFile::ReadOnly))
    {
        fprintf(stderr, "Could not open file: %s\n", inFile);
        return false;
    }

    if (!out.open(QFile::WriteOnly))
    {
        fprintf(stderr, "could not open file: %s\n", outFile);
        in.close();
        return false;
    }

    QByteArray decryptedData = in.readAll();

    if (out.write(decryptedData) != decryptedData.length())
    {
        fprintf(stderr,"Failed to write decrypted file.\n");
    }
    else
    {
        fprintf(stdout, "Encrypted content written to file: %s\n\n", outFile);
    }

    out.close();
    in.close();

    return true;
}
