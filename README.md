# AI Project on OpenNN

<h3>Prawidłowa instalacja QT w skrócie</h3>

<p>Polecam QT z zestawem narzędzi VS2015/64 ale tutaj należy nie zapomnieć, że VS2015 musi być zainstalowany w systemie wcześniej! Jak sprawdzić czy działa poprawnie i mamy w ogóle zainstalowanego C++, a nie tylko C#? Najlepiej otworzyć VS2015 i spróbować stworzyć nowy projekt C++, wtedy jeżeli nie ma odpowiednich paczek, zaproponuje update, który trzeba ściągnąć!</p>

<p>(QT) Właściwy instalator to instalator Offline! Sciągamy go z <a href="https://info.qt.io/download-qt-for-application-development">download-qt-for-application-development</a> -> Get your open source package -> View All Downloads (na samym dole) -> znaleźć wybraną paczkę offline instalatora i ją pobrać.</p>

<p> Jeżeli spełniliśmy powyższe punkty to możemy teraz zainstalować QT, po instalacji, dany zestaw narzędzi w QT (VS2013/15/32 lub 64) nie może być podświetlony na czerwono! Jeżeli nie jest to znaczy, że wszystko przebiegło prawidłowo i mamy wszystko prawidłowo.</p>

<h3> Uruchamianie Aplikacji na OpenNN </h3>
<p> Po pullowaniu repozytorium oraz włączeniu QT możemy teraz otworzyć projekt. Tradycyjnie File->Open Project-> i tutaj plik opennn.pro z głównego katalogu repozytorium </p>

<p>Teraz rozwijamy sobie wszystkie główne katalogi w QT i kompilujemy podprojekty w odpowiedniej kolejności. <b>W kolejności tinyxml2 -> opennn -> examples/logical_operators -> aiproject</b> A robimy to tak że klikamy kursorem na plik *.pro w danym podprojekcie i w górnym menu Budowanie -> klikamy "Zbuduj podprojekt .nazwapodprojektu."</p>

<p>Możliwe że wcześniej (przed wszystkim) trzeba kliknąć w menu Budowanie "uruchom qmake"!</p>
<p> I teraz możemy już wybrać jaki projekt w jakim trybie będzie budowany zieloną strzałeczką jak poniżej</p>

<img src="http://i.imgur.com/nZMguw5.png?1" alt="IMG Choose Target Build" />

<p>Na tą chwile są to dwa podprojekty - jeden to przykładowy, a drugi to projekt</p>
<p>Projekt powinien teraz dać się uruchomić wspomnianą strzałeczką</p>
