#include <stdio.h>
#include <stdbool.h> // Para usar el tipo de dato bool
typedef char cadena[200]; // Para tratar a los arrays de chars como 'cadena'

///////////////////////////////////////////////////////////////////////////
////////////////////////////  PROTOTIPOS  /////////////////////////////////
///////////////////////////////////////////////////////////////////////////

int leerEntero(cadena mensaje);
float leerFloat(cadena mensaje);
char leerCaracter(cadena mensaje);
int leerEnteroEntre(int valorMin, int valorMax, cadena mensaje);
float leerFloatEntre(float valorMin, float valorMax, cadena mensaje);
bool confirmaUsuario(cadena mensaje);

///////////////////////////////////////////////////////////////////////////
/////////////////////////  IMPLEMENTACIONES  //////////////////////////////
///////////////////////////////////////////////////////////////////////////

int leerEntero(cadena mensaje) {
    /* Muestra el mensaje al usuario, lee un entero y lo retorna */
    int entero;
    printf("%s: ", mensaje);
    scanf("%d", &entero);
    return entero;
}   
float leerFloat(cadena mensaje) {
    /* Muestra el mensaje al usuario, lee un float y lo retorna */
    // ¡Esta va resuelta de regalo! 😉
    float numero;
    printf("%s: ", mensaje);
    scanf("%f", &numero);
    return numero;
}

char leerCaracter(cadena mensaje) {
    char caracter;
    printf("%s: ", mensaje);
    scanf(" %c", &caracter);
    return caracter;
}

int leerEnteroEntre(int valorMin, int valorMax, cadena mensaje) {
int numero;
    do {
        printf("%s (de %d a %d): ", mensaje, valorMin, valorMax);
        scanf("%d", &numero);
        if (numero < valorMin || numero > valorMax) {
            printf("ERROR. NUMERO INVALIDO, REINGRESAR UN VALOR ENTRE %d y %d \n", valorMin, valorMax);
        }
    } while (numero < valorMin || numero > valorMax);
    return numero;
}

float leerFloatEntre(float valorMin, float valorMax, cadena mensaje) {
    printf("%s: ", mensaje);
float numero;
    do {
        printf("%s (de %f a %f): ", mensaje, valorMin, valorMax);
        scanf("%f", &numero);
        if (numero < valorMin || numero > valorMax) {
            printf("ERROR. NUMERO INVALIDO, REINGRESAR UN VALOR ENTRE %d y %d \n", valorMin, valorMax);
        }
    } while (numero < valorMin || numero > valorMax);
    return numero;
}

bool confirmaUsuario(cadena mensaje) {
    /* Muestra el mensaje al usuario junto a la leyenda "[S/N]" que
       representa "Si" o "No". Luego, lee chars mientras éstos no sean los
       previstos ('S' o 'N', incluyendo minúsculas) informando al
       usuario de su error. Retorna si el usuario seleccionó que sí. */
        char opcion; 

       printf("%s: [S/N]\n", mensaje);
       scanf(" %c", &opcion);
        if ((opcion >= 65 && opcion <= 77) || (opcion >= 79 && opcion <= 82) || (opcion >= 84 && opcion <= 90) || (opcion >= 97 && opcion <= 109) || (opcion >= 111 && opcion <= 114) || (opcion >= 116 && opcion <= 122))
       {
        do 
        {
            printf("ERROR. CARACTER NO ES VALIDO.\n");
            scanf(" %c", &opcion);
        }
        while ((opcion >= 65 && opcion <= 77) || (opcion >= 79 && opcion <= 82) || (opcion >= 84 && opcion <= 90)  || (opcion >= 97 && opcion <= 109) || (opcion >= 111 && opcion <= 114) || (opcion >= 116 && opcion <= 122)); 
       }

       if (opcion == 83 || opcion == 115)
       {
        return true;
       } 
       else if ((opcion == 78) || (opcion == 110))
       {
        return false;
       }

       
}