# Cours de Programmation Concurrente

## Organisation

- Introduction aux concepts de base tels que le parallélisme vs la concurrence, la synchronisation, la communication inter-processus, et les problèmes liés à la concurrence.
- Diner des philosophes: Un exemple classique de problème de synchronisation, qui peut être utilisé pour introduire les concepts de base de la programmation concurrente.
- Le problème du barbier endormi: Un autre exemple classique de problème de synchronisation, qui peut être utilisé pour introduire les concepts de base de la programmation concurrente.
- Le problème des producteurs-consommateurs
- Le problème de la cigarette des fumeurs : Dans ce scénario, il y a un agent et trois fumeurs. Chaque fumeur a un ingrédient (papier, tabac ou allumettes) et l'agent place aléatoirement deux des trois ingrédients sur la table. Le fumeur ayant le troisième ingrédient nécessaire fabrique et fume une cigarette, signalant ensuite à l'agent de mettre deux autres ingrédients aléatoires sur la table.
- Deadlocks et Livelocks : Les deadlocks surviennent lorsque deux threads ou plus attendent indéfiniment que l'autre libère une ressource. Les livelocks sont des situations similaires, mais avec les processus ou les threads continuant d'exécuter sans progression. Il est important de concevoir des systèmes qui peuvent détecter et récupérer de ces situations, ou mieux encore, les prévenir.
- Accès Atomique aux Données Partagées : Lorsque plusieurs threads ou processus accèdent et modifient des données partagées, il est crucial de s'assurer que ces opérations sont atomiques. Cela signifie que chaque opération sur les données partagées doit être complète sans interruption. Les mutex (verrous exclusifs) et les sémaphores sont souvent utilisés pour garantir cet accès atomique.
- Conditions de Course : Il est important de prévenir les conditions de course, qui se produisent lorsque plusieurs threads ou processus accèdent ou modifient des données partagées dans un ordre qui conduit à des résultats incohérents ou inattendus. La synchronisation correcte est essentielle pour éviter ces problèmes.
- Starvation : La starvation se produit lorsqu'un thread ou un processus n'est jamais capable d'acquérir une ressource nécessaire. Il est important d'implémenter des mécanismes d'équité pour s'assurer que chaque thread ou processus a une chance d'accéder aux ressources nécessaires.
- Outre les threads standard, vous pouvez également aborder les threads légers (par exemple, avec les bibliothèques Pthreads) et les threads basés sur des tâches (par exemple, avec la bibliothèque C++20 `std::jthread`).
- En plus des mutex et des sémaphores, vous pourriez discuter d'autres primitives de synchronisation telles que les barrières, les variables conditionnelles, les verrous de spin, etc.
- Comment concevoir des programmes concurrents de manière efficace et sûre, en évitant les problèmes courants tels que les conditions de concurrence, les courses, les deadlocks, etc.
- Comment gérer efficacement les ressources partagées, telles que la mémoire, dans un environnement concurrent.
- Introduction à la programmation asynchrone et aux promesses/futures (utilisées en C++ avec std::future et std::promise).
- En plus de Pthreads, vous pouvez présenter des bibliothèques modernes de programmation concurrente, comme std::thread en C++11 et les fonctionnalités améliorées en C++20, telles que les coroutines (std::coroutine) et les "latches" (std::latch).
- L'enseignement de la programmation sur GPU avec des compute shaders peut être une excellente idée. Cela peut introduire les étudiants à la programmation hautement parallèle et au calcul général sur GPU.
- Étude de cas: Des exemples concrets de problèmes résolus de manière concurrente, tels que la parallélisation d'algorithmes de tri, la résolution de problèmes complexes avec des threads, etc. (Algorithme de tri bitonique, tri parallèle, etc.)
- Présentation d'outils et de bibliothèques utiles pour la programmation concurrente, tels que std::async, std::threadpool, ou des bibliothèques de haut niveau comme Intel TBB (Threading Building Blocks) ou OpenMP.
- Thread Safety ? : Il est important de noter que les threads ne sont pas toujours la meilleure solution pour la programmation concurrente. Dans certains cas, il peut être préférable d'utiliser des processus, des tâches, des événements, des canaux, des flux, etc. Il est important de choisir la bonne solution pour chaque problème.

## Discussion

### Dekket et Peterson

L'étude des algorithmes de Dekker et Peterson est historiquement importante dans le domaine de la programmation concurrente, car ils sont parmi les premiers algorithmes développés pour résoudre le problème de la section critique et la course aux ressources partagées dans les environnements multithread. Cependant, leur utilisation dans la pratique moderne est relativement limitée, car ils présentent des limitations et des inconvénients importants par rapport aux solutions de synchronisation plus récentes.

Il peut être utile d'introduire brièvement Dekker et Peterson pour donner aux étudiants une compréhension historique de la programmation concurrente et des concepts de base de la synchronisation. Cela peut également aider à expliquer pourquoi des solutions plus modernes et efficaces ont été développées par la suite.

Cependant, il est important de noter que Dekker et Peterson sont des algorithmes relativement simples et ne sont généralement pas utilisés dans des applications réelles en raison de leurs limitations. Les étudiants peuvent bénéficier davantage de l'apprentissage des concepts et des techniques de synchronisation plus modernes et robustes, tels que l'utilisation de mutex, de sémaphores, de variables conditionnelles, etc.

Dans l'ensemble, il est utile d'inclure Dekker et Peterson dans le contexte de l'histoire de la programmation concurrente, mais cela ne devrait pas être le point central de votre cours. Concentrez-vous davantage sur les techniques de synchronisation actuelles et les meilleures pratiques pour gérer la concurrence de manière efficace et sûre dans les applications modernes.

### Bibliothèques disponibles

- [Pthreads](https://en.wikipedia.org/wiki/POSIX_Threads) (POSIX Threads) est une bibliothèque de threads standard pour les systèmes d'exploitation de type Unix. Il est disponible sur la plupart des systèmes d'exploitation modernes, y compris Linux, macOS et Windows (via Cygwin).
- [OpenMP](https://en.wikipedia.org/wiki/OpenMP) est une API de programmation parallèle pour les langages C, C++ et Fortran. Il est disponible sur la plupart des systèmes d'exploitation modernes, y compris Linux, macOS et Windows (via Cygwin).
- [thread](https://en.cppreference.com/w/cpp/thread) est une bibliothèque de threads standard pour C++11 et les versions ultérieures. Il est disponible sur la plupart des systèmes d'exploitation modernes, y compris Linux, macOS et Windows.
- [std::jthread](https://en.cppreference.com/w/cpp/thread/jthread) est une bibliothèque de threads standard pour C++20 et les versions ultérieures. Il est disponible sur la plupart des systèmes d'exploitation modernes, y compris Linux, macOS et Windows.
- [std::async](https://en.cppreference.com/w/cpp/thread/async) est une bibliothèque de threads standard pour C++11 et les versions ultérieures. Il est disponible sur la plupart des systèmes d'exploitation modernes, y compris Linux, macOS et Windows.
- [std::future](https://en.cppreference.com/w/cpp/thread/future) est une bibliothèque de threads standard pour C++11 et les versions ultérieures. Il est disponible sur la plupart des systèmes d'exploitation modernes, y compris Linux, macOS et Windows.
- [std::promise](https://en.cppreference.com/w/cpp/thread/promise) est une bibliothèque de threads standard pour C++11 et les versions ultérieures. Il est disponible sur la plupart des systèmes d'exploitation modernes, y compris Linux, macOS et Windows.
- [std::latch](https://en.cppreference.com/w/cpp/thread/latch) est une bibliothèque de threads standard pour C++20 et les versions ultérieures. Il est disponible sur la plupart des systèmes d'exploitation modernes, y compris Linux, macOS et Windows.
- [std::barrier](https://en.cppreference.com/w/cpp/thread/barrier) est une bibliothèque de threads standard pour C++20 et les versions ultérieures. Il est disponible sur la plupart des systèmes d'exploitation modernes, y compris Linux, macOS et Windows.
- [std::coroutine](https://en.cppreference.com/w/cpp/language/coroutines) est une bibliothèque de threads standard pour C++20 et les versions ultérieures. Il est disponible sur la plupart des systèmes d'exploitation modernes, y compris Linux, macOS et Windows.
