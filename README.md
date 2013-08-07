##  Squirrel(鼠鬚管) Input Method for TW Users

fork from [Squirrel project](http://github.com/lotem/squirrel) written by [佛振(lotem)](https://github.com/lotem) , a derivation version for Taiwan Users

build steps:

open terminal 

	brew install cmake boost glog yaml-cpp kyoto-cabinet opencc

then

	cd /the_folder_of_squirrel_you_just_clone/librime 
	make

waiting… (about 5 min ), few lines of info will appear in terminal 

if the final line states:

	[100%] Built target rime_dict_manager
	:)
congratulation !

go to `the_folder_of_squirrel_you_just_clone`

open **Squirrel.xcodeproj** and **build**
