# Gift-Movie-Tickets
Create an application through which one can gift movie tickets to a guest. The
application must work in the following manner:
1. Assume that the movie tickets are available for only a single show. Multiple dates and
shows are not required to consider.
2. The tickets are of three categories: Normal, Deluxe, and Premium. You can book
tickets for only one category at a time. Assume that the number of total tickets available
are 100, 50, and 30 for Normal, Deluxe and Premium respectively.
3. To purchase tickets one has to place a request to the server along with the email id of
the guest, quantity, and the category of the tickets. Note that you do not need to
implement any online payment methods here.
4. The server will book tickets and send the necessary information (Ticket numbers and
total tickets) to the guest through email.
5. In case of any error during booking the error message will only display on the server
side. No acknowledgement (positive or negative) needs to be communicated to the
person who has requested to book the ticket.


Assume that the person who requests for booking is Client and the person who receives the
ticket through email is Guest. The booking process is maintained in a Server.
1. Implement a Server (listening at HTTP port) using C and run it in your local machine.
The server must be ready to accept new client requests any time. It should not wait for
the completion of any current task before accepting a new task. The technical meaning
of this is, you have to create a separate thread for every new request arrives at the
server. Also, you must terminate the thread when the task is completed. On receiving a
request, the server will book the tickets for the guest if enough tickets are available and
then send the details to the Guest through email.
2. No coding is required for Client and Guest. To send a booking request to the server you
can use the Curl command. Curl command can be used (from a terminal) to send HTTP
POST/GET requests to a server. The request must have the following parameters:
Guest Name, Quantity, and Guest_Email. Basically a separate terminal will act as a
Client and it will send requests to the server through the Curl command.
