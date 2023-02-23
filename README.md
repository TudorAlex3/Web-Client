# Web-Client


### ➢ Client
<p align="justify">
The client always receives input from the keyboard for commands (until the "exit" command is entered). We have verified that each command entered from the keyboard is valid, and based on the received command, we send the request specified in the requirement to the server and display the response sent by the server.
</p>

### ➢ Register & Login
<p align="justify">
These two commands are very similar. For both, a username and password are entered from the keyboard. After this, the "compute_post_request" function is called with the specific route for each command. The server's response is then displayed, along with any associated errors/success messages.
</p>

### ➢ Enter library & Get books & Logout
<p align="justify">
These commands assume that a user is logged in (so none of these commands can be used otherwise). For these, a "GET" request is made with the access route specified in the requirement for each.
</p>

### ➢ Get & Delete book
<p align="justify">
The structure for these is identical. An ID is entered from the keyboard (and it is verified that a valid number has been entered). The entered ID is added to the book access route, and based on the command, a request is made with this new route: for "get_book," a "GET" request is made, and for "delete_book," a "DELETE" request is made, as specified in the requirement.
</p>

### ➢ Add book
<p align="justify">
For this function, we read all the details from the keyboard to create a new book, and then we created a new json using this data. We made a "POST" request to the server with this json and displayed the information received from the server.
</p>
