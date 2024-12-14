TCP/IP protokolü kullanarak bir server (sunucu) ve client (istemci) arasında bağlantı kuran bir quiz (soru-cevap) uygulamasıdır.
Sunucu, istemcilere rastgele sorular gönderir ve verilen cevapları kontrol ederek puanlama yapar. Aynı IP adresine bağlı
birden fazla istemciyle çalışabilir. Her istemci, soruları cevapladıktan sonra puanını sunucudan alır. Proje, ağ programlaması,
çoklu iş parçacığı kullanımı (multithreading) ve veri iletişimi 
konularında temel bir uygulama sunar.

Projenin Özellikleri:
TCP/IP Bağlantısı: Sunucu ve istemci arasında güvenilir bir bağlantı sağlar.
Çoklu İstemci Desteği: Aynı anda birden fazla istemciye hizmet verebilir.
Rastgele Soru Gönderimi: Sorular her istemci için karıştırılarak rastgele sırayla gönderilir.
Puanlama Sistemi: İstemcilerin verdiği doğru cevaplar sunucu tarafından değerlendirilir ve sonuç istemciye iletilir.
Thread Kullanımı: Her istemci bağlantısı için ayrı bir iş parçacığı (thread) oluşturulur.
Dinamik İletişim: Sorular ve cevaplar buffer üzerinden sunucu ve istemci arasında dinamik olarak iletilir.

This project is a quiz application that establishes a connection between a server and client using the TCP/IP protocol.
The server sends random questions to clients, evaluates their responses, and calculates their scores.
It supports multiple clients connected from the same IP address.
Each client receives their score from the server after answering the questions.
This project serves as a fundamental implementation of network programming, multithreading, and data communication.

Features of the Project:
TCP/IP Connection: Provides a reliable connection between the server and clients.
Multi-Client Support: Can handle multiple clients simultaneously.
Random Question Distribution: Questions are shuffled and sent in a random order for each client.
Scoring System: The server evaluates the correct answers from the clients and calculates their scores.
Threading: A separate thread is created for each client connection.
Dynamic Communication: Questions and answers are dynamically exchanged between the server and client using a buffer.