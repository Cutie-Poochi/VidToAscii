Make sure you have g++ (or any c++ compiler like visual c++ on windows), ImageMagick and ffmpeg set up, also set up Magick++ from ImageMagick.
You have to compile, link and run the file run.cpp.
Some of the commands differ per operating system, you can try searching up their equivalents for your one.
I don't plan on updating this much so ask others for your errors.
Change the names of these options if your options are different but first try compiling them
to make sure it works before substituting the files and these names, if it doesn't, fix it before switching.

Warning: Do not use files with spaces in their names, it will only detect the first word.

InputFile: input.mp4				<-- video input
CharacterImages: Characters.png		<-- character map to detect
OutputFile: output.txt				<-- text file where it will store characters
Fps: 30								<-- fps you want to capture, more fps than video isn't needed
CharacterWidth: 128					<-- amount of characters you want in x axis of the terminal
CharacterHeight: 44					<-- amount of characters you want in y axis of the terminal

I am bored of this project, don't use this unless you know c++ or get the help of someone who does.
It's mostly done but I haven't finished the code to display but that shouldn't be too hard if you
know any language. Also this is OS specific, should work fine on most linux distros but Idk about
windows. You might also need to install additional libraries to support ffmpeg/Imagemagick or
change some of the commands yourself.
