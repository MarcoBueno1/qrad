# qrad

The QRad tool is a mini code generator that allows you to create QT projects from parameters passed by the command line. This is a very rudimentary tool and will be improved gradually.

Based in some fields, it can instant create projects with tables, and forms ( search and edit form ). After that, you just need adjust visual fields ( to best design ) and build project via QT.

In Projects\Blink folder there is an project initially created with this tool (will be soon moved to another repository)

NOTE: Remember, this is a very small, rudimentary and non professional tool, use it on your own risk.


To Build (Windows - Qt 5.10.1)

1. Open QT Terminal ( to build via command line )
2. run: QMake
3. run: mingw32-make
4. run: mingw32-make install

To Use (example)

1. cd bin
2. run: qrad -s User -t User -c id -i int -c name.Name -i QString:searchmaster -c gender.Gender -i int:multi:Gender.Description[Male,Female] -c birthday -i QDate -c registration -i QTime
3. cd built\user
4. qmake
5. mingw32-make
6. mingw32-make install
7. cd .. \ ..\
8. run: qradplugintest built\user\release\Userplugin.dll

