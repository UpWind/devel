NMEAClock contains some tips (extra comments) on how to implement new plugins.

PluginWizard can generate a dummy plugin class that is ready to be compiled.

Notice that plugins can work a bit slow in debug mode. You have to compile them in release mode for the release version.

Plugins only with with the same version of Qt in which they were compiled in. Debug versions don't work with release version executable and the opposite is also true. Also event slight changes in the plugins might break them so you have to run a clean, qmake and build.