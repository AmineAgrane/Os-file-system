# Os-file-system
OS Project : Simple file manager that uses a simulated partition (UNIX file) as support to store files and directories. 

## General
The operating system project aimed to design a C file manager on a partition represented by a Unix file.
This objective has been achieved. As part of our project, the features available to the user are:
* Creation of a file / folder on the partition.
* Delete a file / folder from the partition.
* File / folder copy between different directories.
* Reading and displaying the contents of a file / folder.
* Writing the content of a file
* Moving the user inside the partition.
* File / folder renaming.
* Closure and exit of the partition.

## Getting started
### Installing and compile
To install this project:
First download the project from :
```
git clone https://github.com/Anclaguedon/Os-file-system
```

Then go to the directory
```
cd os_project
```

Compile source code and generate docs
```
make
make gendocs
```
To run
```
./programme
```

## File system structure
<img src="https://github.com/Anclaguedon/Os-file-system/blob/master/docs/file_system_structure.png" width="290" height="530">
The partition (Unix file) is divided into N blocks of equal size.

## More
Check the doc to understand the code
