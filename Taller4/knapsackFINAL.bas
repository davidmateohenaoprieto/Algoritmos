REM Definición de la función knapSack
DECLARE SUB knapSack (pesoMax AS INTEGER, listPesos() AS INTEGER, listVal() AS INTEGER, numItems AS INTEGER)
DEFINT A-Z

REM Definición de la subrutina main
DECLARE SUB main ()
DEFINT A-Z

REM Función knapSack

REM Inicio del programa
main


SUB knapSack (pesoMax AS INTEGER, listPesos() AS INTEGER, listVal() AS INTEGER, numItems AS INTEGER)
    DIM valoresMaximos(numItems, pesoMax)
    FOR i = 0 TO numItems
        FOR j = 0 TO pesoMax
            IF i = 0 OR j = 0 THEN
                valoresMaximos(i, j) = 0
            ELSEIF listPesos(i - 1) <= j THEN
                IF listVal(i - 1) + valoresMaximos(i - 1, j - listPesos(i - 1)) > valoresMaximos(i - 1, j) THEN
                    valoresMaximos(i, j) = listVal(i - 1) + valoresMaximos(i - 1, j - listPesos(i - 1))
                ELSE
                    valoresMaximos(i, j) = valoresMaximos(i - 1, j)
                END IF
            ELSE
                valoresMaximos(i, j) = valoresMaximos(i - 1, j)
            END IF
        NEXT j
    NEXT i

       
    REM Backtracking
    result = valoresMaximos(numItems, pesoMax)
    WHILE pesoMax > 0 AND numItems > 0
        IF valoresMaximos(numItems, pesoMax) <> valoresMaximos(numItems - 1, pesoMax) THEN
            PRINT "Item: "; numItems; " Peso: "; listPesos(numItems - 1); " Valor: "; listVal(numItems - 1)
            pesoMax = pesoMax - listPesos(numItems - 1)
            numItems = numItems - 1
        ELSE
            numItems = numItems - 1
        END IF
       
    WEND

    PRINT "Valor Maximo: "; result

END SUB

REM Subrutina main
SUB main

    DIM listPesos(100) AS INTEGER
    DIM listVal(100) AS INTEGER

    REM /////////////////////////////
    DIM peso AS INTEGER
    DIM valor AS INTEGER
    DIM values(1) AS STRING
    REM /////////////////////

    posit = INSTR(2, COMMAND$, SPACE$(1)) 'find position of spacer
    arg1$ = MID$(COMMAND$, 1, posit - 1) 'first number
    arg2$ = MID$(COMMAND$, posit + 1) 'second number


    filename$ = arg1$

    pesoMax = VAL(arg2$)

    REM pesoMax = 50
    REM filename$ = "items.txt"

    OPEN filename$ FOR INPUT AS #1
    PRINT "-------------Informacion del archivo-------------"
    i = 0
    DO WHILE NOT EOF(1)
        LINE INPUT #1, line$

        posit = INSTR(2, line$, SPACE$(1)) 'find position of spacer
        num1$ = MID$(line$, 1, posit - 1) 'first number
        num2$ = MID$(line$, posit + 1) 'second number
        peso = VAL(num1$) 'convert to a real numbers
        valor = VAL(num2$) 'VAL will ignore leading space

        REM PRINT "Numero 1"; peso
        REM PRINT "Numero 2"; valor

        REM PRINT line$
        listPesos(i) = peso
        listVal(i) = valor

        PRINT "Item"; i + 1; " Peso: "; peso; " Valor: "; valor
        i = i + 1
    LOOP
    CLOSE #1

    numItems = i
    PRINT "-------------Solucion-------------"
    knapSack pesoMax, listPesos(), listVal(), numItems

END SUB

