/**
 * @file protocolo.h
 * @brief Librería del protocolo de comunicación de los robots
 * @author Kevin Hernández, Ángel Gil
 */
#ifndef PROTOCOLO
#define PROTOCOLO

class protocolo{
public:
    enum importante{
        tam_min = 5
    };

    enum comportamientos{
        Explorar           = 'A',
        Evadir             = 'B',
        Seguir_instruccion = 'C',
        Detener            = '!'
    };
    enum robot_defecto{
        todos_robots = 'X'
    };
    enum caracteres_intermedios{
         delimitador_i = '<',  //Delimitador inicial de mensajes
         delimitador_f = '>',  //Delimitaador final de mensajes
         separador     = ':'  //Separador de los mensajes
    };
    enum control_robot{
        buscar             = '?',
        excepcion          = 'E',
        arriba             = '1',
        abajo              = '2',
        derecha            = '3',
        izquierda          = '4',
        sensor_distancia   = 0,
        sensor_infrarojo   = 1,
        numero_excepciones = 2
    };
    enum joystick_c{
         eje_1       = 0,
         eje_2       = 1,
         rango       = 27000,
         adelante_j  = 1,
         atras_j     = 2,
         derecha_j   = 3,
         izquierda_j = 4
    };
};

#endif // PROTOCOLO

