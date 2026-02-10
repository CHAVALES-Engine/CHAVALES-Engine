## Descripción:
El módulo de recursos será el encargado de gestionar la carga y descarga de recursos en una pequeña caché.

El módulo de recursos mapeará las ids internas de los recursos con un nombre (string) asociado, una vez cargado el juego el módulo de recursos recorre recursivamente la carpeta de assets mapeando las rutas a los ficheros con su id asociada.
A petición del desarrollador, el módulo de recursos carga 1 vez cada recurso en su mapa correspondiente con la id indicada

---
## Gestor de recursos de OGRE
Para lidiar con el gestor de recursos de ogre, se gestionarán los recursos para ogre en una clase propia. Estas clases envolverán los recursos de ogre y contendrán un puntero a los mismos, así conseguimos que coexistan ambos gestores de recursos. Por otro lado, ogre será el encargado de gestionar la memoria, mientras que el módulo de recursos será el encargado de gestionar la instancia.

Código usado para parsear "resources.cfg" de ogre:
```C++
// Carga los paths de recursos del archivo de configuración
ConfigFile cf;
cf.load("resources.cfg");

// 
ConfigFile::SectionIterator seci = cf.getSectionIterator();

String secName, typeName, archName;
while (seci.hasMoreElements()){
	secName = seci.peekNextKey();
	ConfigFile::SettingsMultiMap *settings = seci.getNext();
	ConfigFile::SettingsMultiMap::iterator i;
	for(i = settngs->begin(); i!= settings->end(); ++i){
		typeName = i->first;
		archName = i->second;
		ResourceGroupManager::getSingleton().addResourceLocation(
			archName, typeName, secName);
	}
}
```
(Código del libro "Pro OGRE 3D Programming")

Codigo para hardcodear el "resources.cfg"
```C++
ResourceGroupManager *rgm = ResourceGroupManager::getSingletonPtr();
rgm->addResourceLocation("../media/packs/OgreCore.zip", "Zip", "Bootstrap")
rgm->addResourceLocation("../media", "FileSystem", "General")
rgm->addResourceLocation("../media/fonts", "FileSystem", "General")
```
(Código del libro "Pro OGRE 3D Programming")

Una vez añadidas las locations, OGRE inicializa los recursos usando
``` C++
// Inicializa los grupos definidos anteriormente
ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
// Inicializa manualmente cada grupo
ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
ResourceGroupManager::getSingleton().initialiseResourceGroup("Bootstrap");
```
(Código del libro "Pro OGRE 3D Programming")

Este código puede sernos útil a la hora de cargar recursos en nuestro módulo de recursos, asi conseguimos 