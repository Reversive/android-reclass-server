# android-reclass-server
Android TCP server for android-reclass-plugin (https://github.com/Reversive/android-reclass-plugin)
## Compile
```
ndk-build
```
## Installation & Usage
You will need a rooted phone to be able to call the reading syscalls, place the compiled executable on `/data/local/tmp` (or anywhere where executable permissions are mounted)
```
adb push reclass-server /data/local/tmp
```
Before executing your file remember to forward the tcp port you are going to use
```
adb forward tcp:<port> tcp:<port>
adb shell
$> su
#> cd /data/local/tmp
#> chmod +x reclass-server
#> ./reclass-server <port>
```
