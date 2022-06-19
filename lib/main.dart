import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:esp32_firebase/dashboard.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:flutter/material.dart';

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      home: ESPTemp(),
    );
  }
}

class ESPTemp extends StatefulWidget {
  const ESPTemp({Key? key}) : super(key: key);

  @override
  State<ESPTemp> createState() => _ESPTempState();
}

class _ESPTempState extends State<ESPTemp> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Sensor List"),
      ),
      body: StreamBuilder<QuerySnapshot<Map<String, dynamic>>>(
        stream: FirebaseFirestore.instance.collection("sensors").snapshots(),
        builder: (context, snapshot) {
          if (snapshot.hasError) {
            return Center(
              child: Text(snapshot.error.toString()),
            );
          }

          if (!snapshot.hasData) {
            return const Center(child: CircularProgressIndicator());
          }

          final data = snapshot.requireData;

          return ListView.builder(
            itemCount: data.size,
            itemBuilder: (context, index) {
              return ListTile(
                title: Text(data.docs[index].data()['title']),
                onTap: () => Navigator.push(
                  context,
                  MaterialPageRoute(
                    builder: (context) => Dashboard(
                      title: data.docs[index].data()['title'],
                      documentID: data.docs[index].id,
                    ),
                  ),
                ),
              );
            },
          );
        },
      ),
    );
  }
}
