The application is a client server application. you have to have two programs, 1. client side and 2. server side.

client side provides user interface, takes user input or takes user request and either POST's the information to server to save or GETs the information to show on the client side.

server side accepts requests from clients, takes the inputs, processes and responds to the client for all requests. some requests may not need response body (if client is sending a POST) except ACK. some requests need to send the data needed by client.

student details are name, roll number, address, father name, mother name

student marks are for 6 subjects (of your choice) for two years only (12 subjects). student can post or get marks by each year (year1, year2)

student files are any attachments they want to save.

client provides UI to enter student details or GET student details given the roll number. all fields when entering are mandatory. for students marks you can view given roll number and year1 or year2 as input. or POST giving roll number and marks and year1 or year2 as input. client provides the input UI for all the above options

server takes the roll number as input and returns student details or takes roll number and address details and does save in a database. database is a 1MB file that you create (like you did earlier in file system for messages) and student details and marks are in that database. similarly server takes roll number and year1 or year2 as input and returns marks or takes all marks/rollno/year1 or year2 as input and save in the database.

student can post files or get files. when they do that activity client transfers the files to server (similar to ftp) and saves them in file system. for each student by roll number you can have a file names or folders creatted on server. when student want to GET and if the file is present, its transfered to the client local directory.
file creation, viewing is not part of this excercise. any file can be transfered using this method.

database is a 1MB file that you create and student details and marks are in that database. similarly server takes roll number and year1 or year2 as input and returns marks or takes all marks/rollno/year1 or year2 as input and save in the database.

student can post files or get files. when they do that activity client transfers the files to server (similar to ftp) and saves them in file system. for each student by roll number we have a directory.
