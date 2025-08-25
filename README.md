# hopfield_network

##Cosa fa il programma

##Librerie usate
- **SFML** – gestione grafica e visualizzazione dei pattern  
- **CMake** – sistema di build  
- **Ninja** – sistema di compilazione ad alte prestazioni  
- **doctest** – framework per i test unitari  

Installazione su Debian/Ubuntu:
sudo apt install libsfml-dev
sudo apt install cmake
sudo apt install ninja-build

##Come si compila
Per generare l'eseguibile con CMake e Ninja si usano i seguenti comandi:

cmake -S . -B build -G "Ninja Multi-Config"
# Compilazione Debug
cmake --build build --config Debug       
cmake --build build --config Debug --target test
# Compilazione Release
cmake --build build --config Release     
cmake --build build --config Release --target test

Dopo la compilazione eseguire:
# Modalità Debug
./build/Debug/hopfield_network
# Modalità Release     
./build/Release/hopfield_network    

##Come si usa
Dopo l'avvio, il programma mostra in alto le immagini ridimensionate, in bianco e nero e zoommate.
Cliccando su una di esse ne comparirà la versione corrotta in basso a sinistra.
Cliccando poi sull'immagine corrotta, inizia la dinamica di aggiornamento.

Nota: la corruzione è casuale; per ottenere versioni corrotte diverse è necessario riavviare il programma.
Se si vuole aumentare il livello di rumore/tagli, modificare i parametri nelle funzioni noise, vertical_cut e orizontal_cut in functions.hpp.

Se si vogliono cambiare le immagini:
- accetta solo formato .png
- inserire sempre 4 immagini per volta per la migliore resa grafica
- rimuovere tutte le immagini precedenti dalle cartelle resized, noised, zoomed_W_noise e zoomed manualmente/eliminare direttamente le cartelle
- inserire le immagini preselte nella cartella source
- eliminare le immagini che erano prima nella cartella source
- cambiare a riga 36-37 del file graphics.cpp i nomi delle immagini, sostituendoli con quelli delle immmagini inserite in source
