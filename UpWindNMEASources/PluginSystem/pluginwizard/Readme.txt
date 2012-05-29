Info:

Custom wizards are located in subdirectories of the following directories:

    * share/qtcreator/templates/wizards
    * the local user's configuration folder, $HOME/.config/Nokia/qtcreator/templates/wizards
    * %APPDATA%\Nokia\qtcreator\templates\wizards

For example
C:\Qt\2010.05\share\qtcreator\templates\wizards\


All plugins are compiled as debug versions by default. Change project build options in Qt Creator if you want to build a release version. Some plugins may work very slowly in debug mode.

Installation:

1) Copy this folder (not just the files) inside one of the mentioned subdirectories and restart Qt Creator.
2) You can now create a custom project from Qt Creator
3) File > New File or Project...
4) Custom projects > NMEA Plugin
5) Give the name of the plugin and select the project folder as UpWindNMEA\PluginSystem
6) The project is now ready to be generated.
7) You can now set the plugin build options to release and build it. You should be immediately able to use it in the program.