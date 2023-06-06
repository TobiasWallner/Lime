Function for vertification of files creates folders New, Error and Verified for manual vertification


By calling the function fileVerify(path, filename, content), the function creates the folders "Error", "Verified" and "New" in "path", if they do not already exist.

The function checks if "filename" file exists in "Verified", and compares the content of this file to "content".

If the file does not exist in "Verified", a file with "content" is generated in the folder "New", and it is the users job to look if this file is correct, and move it to the folder "Verified" if it is.

If "filename" exists in "Verified" the file content is checked against "content", and if they are equal, no new file is generated, the function exists with a successfull verification message. 
If "filename"s content is not equal to "content" a new file is generated in "Error". This file will be deleted if the same file is verified later. 

