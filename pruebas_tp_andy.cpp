#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const string PATH_MATERIALES ="materiales.txt";
const string PATH_EDIFICIOS = "edificios.txt";

struct Material{
    string nombre_material;
    int cantidad_material;
};

struct Lista_materiales{
    // int ** matriz; //creo el doble puntero
    Material** materiales;
    int cantidad_de_materiales;   
};

struct Edificio{
    string nombre_edificio;
    int cantidad_piedra;
    int cantidad_madera;
    int cantidad_metal;
    int cantidad_construidos;
    int max_cantidad_permitidos;
};

struct Lista_edificios{
    // int ** matriz; //creo el doble puntero
    Edificio** edificios;
    int cantidad_de_edificios;   
};


void agregar_edificio(Lista_edificios* lista_edificios, Edificio* edificio){
    int tope_viejo = lista_edificios -> cantidad_de_edificios;

    // int ** matriz;
    // matriz = new int*[4];
    Edificio** nuevo_vector_edificios = new Edificio*[tope_viejo + 1];

    // for (int i = 0; i < 4; i++){ //xa cada fila
    for(int i = 0; i < lista_edificios -> cantidad_de_edificios; i++){
        nuevo_vector_edificios[i] = lista_edificios -> edificios[i];
        //matriz[i] = new int[5] };
    }
    nuevo_vector_edificios[tope_viejo] = edificio;

    if(lista_edificios -> cantidad_de_edificios != 0){
        delete[] lista_edificios -> edificios;
    }

    lista_edificios-> edificios = nuevo_vector_edificios;
    lista_edificios -> cantidad_de_edificios++;
};


void cargar_edificios(Lista_edificios* lista_edificios){
    lista_edificios -> cantidad_de_edificios = 0;

    fstream archivo_edificios(PATH_EDIFICIOS, ios::in);

    if(archivo_edificios.is_open()){

        string nombre_edificio;
        int cantidad_piedra;
        int cantidad_madera;
        int cantidad_metal;
        int cantidad_construidos;
        int max_cantidad_permitidos;

        Edificio* edificio;

        while(archivo_edificios >> nombre_edificio){
            archivo_edificios >> cantidad_piedra;
            archivo_edificios >> cantidad_madera;
            archivo_edificios >> cantidad_metal;
            archivo_edificios >> cantidad_construidos;
            archivo_edificios >> max_cantidad_permitidos;

            edificio = new Edificio;
            
            edificio -> nombre_edificio = nombre_edificio;
            edificio -> cantidad_piedra = cantidad_piedra;
            edificio -> cantidad_madera = cantidad_madera;
            edificio -> cantidad_metal = cantidad_metal;
            edificio -> cantidad_construidos = cantidad_construidos;
            edificio -> max_cantidad_permitidos = max_cantidad_permitidos;

            agregar_edificio(lista_edificios, edificio);
        }

        archivo_edificios.close();
    }
    else{
        lista_edificios -> cantidad_de_edificios = -1;//para chequear si se abrio o no el archivo
        cout<<"No se encontro el archivo "<<PATH_EDIFICIOS<<endl;
    }
}


void agregar_material(Lista_materiales* lista_materiales, Material* material){
    int tope_viejo = lista_materiales -> cantidad_de_materiales;

    // int ** matriz;
    // matriz = new int*[4];
    Material** nuevo_vector_materiales = new Material*[tope_viejo + 1];

    // for (int i = 0; i < 4; i++){ //xa cada fila
    for(int i = 0; i < lista_materiales -> cantidad_de_materiales; i++){
        nuevo_vector_materiales[i] = lista_materiales -> materiales[i];
        //matriz[i] = new int[5] };
    }
    nuevo_vector_materiales[tope_viejo] = material;

    if(lista_materiales -> cantidad_de_materiales != 0){
        delete[] lista_materiales -> materiales;
    }

    lista_materiales -> materiales = nuevo_vector_materiales;
    lista_materiales -> cantidad_de_materiales++;
}


void cargar_materiales(Lista_materiales* lista_materiales){
    lista_materiales -> cantidad_de_materiales = 0;

    fstream archivo_materiales(PATH_MATERIALES, ios::in);

    if(archivo_materiales.is_open()){

        string nombre;
        int cant;

        Material* material;

        while(archivo_materiales >> nombre){
            archivo_materiales >> cant;

            material = new Material;
            material -> nombre_material = nombre;
            material -> cantidad_material = cant;

            agregar_material(lista_materiales, material);
        }

        archivo_materiales.close();
    }
    else{
        lista_materiales -> cantidad_de_materiales = -1;//para chequear si se abrio o no el archivo
        cout<<"No se encontro el archivo "<<PATH_MATERIALES<<endl;
    }
}


int main(){

    Lista_materiales*lista_materiales = new Lista_materiales;
    cargar_materiales(lista_materiales);
    
    Lista_edificios*lista_edificios = new Lista_edificios;
    cargar_edificios(lista_edificios);

    if (lista_materiales->cantidad_de_materiales != -1 && lista_materiales->cantidad_de_materiales != 0 &&  
    lista_edificios -> cantidad_de_edificios !=-1 && lista_edificios -> cantidad_de_edificios !=0 ){ 
    //Si se pudieron abrir (!= -1) y no estan vacios (!=0), muestro el menu
    
    cout<<"NOMBRE\t"<<"MATERIAL"<<endl;
    for(int i = 0; i < lista_materiales -> cantidad_de_materiales; i++){
        cout<<lista_materiales -> materiales[i] -> nombre_material<<"\t";
        cout<<lista_materiales -> materiales[i] -> cantidad_material<<endl;
    }

    cout<<endl;
    cout<<"NOMBRE EDIFICIO\t"<<"CANT CONSTRUIDO\t"<<"MAX CANT PERMITIDA"<<endl;
    for(int i = 0; i < lista_edificios -> cantidad_de_edificios; i++){
        cout<<lista_edificios -> edificios[i] -> nombre_edificio <<" \t\t";
        cout<<lista_edificios -> edificios[i] -> cantidad_construidos << " \t\t";
        cout<<lista_edificios -> edificios[i] -> max_cantidad_permitidos<<endl;
    }
    
    }
    //El sig chequeo de si los archivos se encuentran vacios evitan el sig mensaje de error:
    //free(): double free detected in tcache 2
    //Aborted (core dumped)
    //que salta porque estoy liberando memoria de una supuesta matriz que que no he pedido 
    //ya sea por no poder abrir el archivo o porque estaba vacio. 
    //El problema surge al no saber cual es el archivo q causa problemas.

    if (lista_materiales -> cantidad_de_materiales == 0 || lista_materiales -> cantidad_de_materiales == -1){
        if (lista_materiales -> cantidad_de_materiales == 0){
        cout<<"EL ARCHIVO '"<<PATH_MATERIALES<<"' ESTA VACIO"<<endl;
        }
    }
    else{
        //libero el heap q use para la matriz
        //borro las filas de la matriz materiales que hay dentro de lista_materiales

        int cantidad_de_materiales = lista_materiales -> cantidad_de_materiales;

            for(int i = 0; i < cantidad_de_materiales; i++){    //xa cada fila
                delete lista_materiales -> materiales[i];   //se elimina el espacio de la fila
                //delete[] matriz[i]
                lista_materiales -> cantidad_de_materiales--;
            }
            //delete[] matriz;   //se elimina el vector de vectores
            delete[] lista_materiales -> materiales;
            lista_materiales -> materiales = nullptr;
    }

    if (lista_edificios -> cantidad_de_edificios == 0 || lista_edificios -> cantidad_de_edificios ==-1){
        if (lista_edificios -> cantidad_de_edificios == 0){
            cout<<"EL ARCHIVO '"<<PATH_EDIFICIOS<<"' ESTA VACIO"<<endl; 
        }
    }
    else{
        //Libero el heap q use para la matriz de edificios
        //borro las filas de la matriz materiales que hay dentro de lista_materiales

        int cantidad_de_edificios = lista_edificios -> cantidad_de_edificios;

        for(int i = 0; i < cantidad_de_edificios; i++){    //xa cada fila
            delete lista_edificios -> edificios[i];   //se elimina el espacio de la fila
            //delete[] matriz[i]
            lista_edificios -> cantidad_de_edificios--;
        }
        //delete[] matriz;   //se elimina el vector de vectores
        delete[] lista_edificios -> edificios;
        lista_edificios -> edificios = nullptr;
    }
    

    //borro la lista de materiales
    delete lista_edificios;
    //borro la lista de materiales
    delete lista_materiales;

    return 0;

}

// yacimiento 125 280 0 0 2
// escuela 152 298 45 1 1
// fabrica 100 100 258 0 4
// aserradero 100 58 50 1 2


// piedra 200
// metal 250
// madera 500
// oro 130


    