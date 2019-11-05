This client server pair immitates a database that uses GET/POST/DELETE
requests to get and store data. 
This is implemented using a TCP connection between the client and the server, 
the necesary port is 6666

The client can put new constants, conversions, or definitions in the server by
using the following syntax

    PUT *Conversion/Constant/Definition *NecesaryParameters FACTP/0.9

The necesary parameters for each item are as follows:
    Conversion
        - name: the name of the conversion, used for lookup
        - unit1: the orignal unit for the conversion
        - unit2: the new unit for the conversion
        - factor: the relationship between unit1 and unit2
    Constant
        - name: the name of the conversion, used for lookup
        - value: the value of the constant
    Definition
        -name: the word being defined, used for lookup
        -part: the part of speech the word falls under
        -definition: the definition of the word

The client can get the stored data from the server using the following syntax

    GET *Conversion/Constant/Definition FACTP/0.9

The client can delete any parts of the data by using the following syntax

    DELETE *Conversion/Constant/Definition *NameOfItem FACTP/0.9


 [] make so user does not have to type FACTP/0.9
 [] clean up code
 [] add more descriptive variable names
 [] catch more segfaults (not a lot left but some still present)