/**
 * @file protocolo.h
 * @brief Librería del protocolo de comunicación de los robots
 * @author Kevin Hernández, Ángel Gil
 */
#ifndef PROTOCOLO
#define PROTOCOLO
#include <QString>
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
    enum tipo_instruccion{
        Comportamiento = 0,
        Busqueda       = 1,
        Excepcion_tipo = 2,
        Caracteres     = 3
    };

    static QString *cadenas_comportamientos;

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
        adelante           = '1',
        atras              = '2',
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

    static QString getCadenaInstruccion(char instruccion){
        switch(instruccion){
            case Explorar:
                return "Explorar";
            break;
            case Evadir:
                return "Evadir";
            break;
            case Seguir_instruccion:
                return "Seguir instrucciones";
            break;
            case Detener:
                return "Detenido";
            break;
            case adelante:
                return "adelante";
            break;
            case atras:
                return "atras";
            break;
            case derecha:
                return "derecha";
            break;
            case izquierda:
                return "izquierda";
            break;
        }
        return "err";
    }
    static int getTipoInstruccion(char instruccion){
        switch(instruccion){
            case Explorar:
            case Evadir:
            case Seguir_instruccion:
            case Detener:
            case adelante:
            case atras:
            case derecha:
            case izquierda:
                return Comportamiento;
            break;
            case buscar:
                return Busqueda;
            break;
            case excepcion:
                return Excepcion_tipo;
            break;
            break;
            case delimitador_i :
            case delimitador_f:
            case separador:
                return Caracteres;
            break;
        }
    }

    static bool verificacion(QByteArray data, int index){
        /*5 es el tamaño mínimo de los mensajes*/
        if(index < data.size() && data.size() >= protocolo::tam_min){
            switch(data.at(index)){
                case protocolo::delimitador_i:
                    if(!(index == 0
                       &&
                       ((index + 1) < data.size() &&
                        data.at(index + 1) != protocolo::delimitador_i &&
                        data.at(index + 1) != protocolo::delimitador_f &&
                        data.at(index + 1) != protocolo::separador)))
                        return false;
                break;
                case protocolo::delimitador_f:
                    if((index == (data.size() - 1)) &&
                       (index - 1 > 0 &&
                        data.at(index - 1) != protocolo::delimitador_i &&
                        data.at(index - 1) != protocolo::delimitador_f &&
                        data.at(index - 1) != protocolo::separador))
                        return true;
                break;
                case protocolo::separador:
                    if(( (index + 1) < data.size()&&(
                         (data.at(index + 1) == protocolo::delimitador_i) ||
                         (data.at(index + 1) == protocolo::delimitador_f) ||
                         data.at(index + 1) == protocolo::separador))
                       ||
                        ((index - 1) > 0 &&
                         (data.at(index - 1) == protocolo::delimitador_i ||
                         data.at(index - 1) == protocolo::delimitador_f ||
                         data.at(index - 1) == protocolo::separador)))
                        return false;
                break;
                default:
                    if((index == 0 || index == (data.size() - 1)) ||
                      ((index - 1 > 0 &&
                       data.at(index - 1) != protocolo::delimitador_i &&
                       data.at(index - 1) != protocolo::separador)
                       ||
                       (index + 1 < data.size() &&
                       data.at(index + 1) != protocolo::delimitador_f &&
                       data.at(index + 1) != protocolo::separador)))
                       return false;
                break;
            }
            return verificacion(data, index + 1);
        }
    return false;
    }
};

#endif // PROTOCOLO

