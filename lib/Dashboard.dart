import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'CircleProgress.dart';

class Dashboard extends StatefulWidget {
  @override
  _DashboardState createState() => _DashboardState();
}

class _DashboardState extends State<Dashboard>
    with SingleTickerProviderStateMixin {
  bool isLoading = false;
  
  
  //final GoogleSignIn googleSignIn = GoogleSignIn();

  final databaseReference = FirebaseDatabase.instance.ref();

  AnimationController progressController;
  Animation<double> tempAnimation;
  Animation<double> humidityAnimation;
  AnimationController peogressController;

  @override
  void initState() {
    super.initState();

    databaseReference
        .child('ESP32')
        // .onValue.listen((event) {
        //   var snapshot = event.snapshot;
        //   double temp = snapshot.value['Temperature']['Data'];
        //   print('Value is $temp');
        //   double humidity = snapshot.value['Humidity']['Data'];
        //   print('Value is $humidity');

        //   isLoading = true;
        //   _DashboardInit(temp, humidity);
          
        // },);}

     _realdb_one(){
       _dbref.once().then((DataSnapshot dataSnapshot){
         print(object)
       })
     }

      

  //     .once()
  //     .then((DataSnapshot snapshot) {
  //     double temp = snapshot.value['Temperature']['Celsius'];
  //     double humidity = snapshot.value['Humidity']['Fahrenheit'];
  //     json 
      

  //     isLoading = true;
  //     _DashboardInit(temp, humidity);
  //   });
  // }

  // _DashboardInit(double temp, double humid) {
  //   peogressController = AnimationController(
  //       vsync: this, duration: Duration(milliseconds: 5000)); //5s

  //   tempAnimation =
  //       Tween<double>(begin: -50, end: temp).animate(progressController)
  //         ..addListener(() {
  //           setState(() {});
  //         });

  //   humidityAnimation =
  //       Tween<double>(begin: 0, end: humid).animate(progressController)
  //         ..addListener(() {
  //           setState(() {});
  //         });

  //   progressController.forward();
  // }

  // @override
  // Widget build(BuildContext context) {
  //   return Scaffold(
  //     appBar: AppBar(
  //       title: Text('Dashboard'),
  //       centerTitle: true,
  //       automaticallyImplyLeading: false,
  //       // leading: new IconButton(
  //       //     icon: Icon(Icons.reorder), onPressed: ),
  //     ),
  //     body: Center(
  //         child: isLoading
  //             ? Column(
  //                 mainAxisAlignment: MainAxisAlignment.spaceEvenly,
  //                 children: <Widget>[
  //                   CustomPaint(
  //                     foregroundPainter:
  //                         CircleProgress(tempAnimation.value, true),
  //                     child: Container(
  //                       width: 200,
  //                       height: 200,
  //                       child: Center(
  //                         child: Column(
  //                           mainAxisAlignment: MainAxisAlignment.center,
  //                           children: <Widget>[
  //                             Text('Temperature'),
  //                             Text(
  //                               '${tempAnimation.value.toInt()}',
  //                               style: TextStyle(
  //                                   fontSize: 50, fontWeight: FontWeight.bold),
  //                             ),
  //                             Text(
  //                               '°C',
  //                               style: TextStyle(
  //                                   fontSize: 20, fontWeight: FontWeight.bold),
  //                             ),
  //                           ],
  //                         ),
  //                       ),
  //                     ),
  //                   ),
  //                   CustomPaint(
  //                     foregroundPainter:
  //                         CircleProgress(humidityAnimation.value, false),
  //                     child: Container(
  //                       width: 200,
  //                       height: 200,
  //                       child: Center(
  //                         child: Column(
  //                           mainAxisAlignment: MainAxisAlignment.center,
  //                           children: <Widget>[
  //                             Text('Humidity'),
  //                             Text(
  //                               '${humidityAnimation.value.toInt()}',
  //                               style: TextStyle(
  //                                   fontSize: 50, fontWeight: FontWeight.bold),
  //                             ),
  //                             Text(
  //                               '%',
  //                               style: TextStyle(
  //                                   fontSize: 20, fontWeight: FontWeight.bold),
  //                             ),
  //                           ],
  //                         ),
  //                       ),
  //                     ),
  //                   )
  //                 ],
  //               )
  //             : Text(
  //                 'Loading...',
  //                 style: TextStyle(fontSize: 30, fontWeight: FontWeight.bold),
  //               )),
  //   );
  // }

  // handleLoginOutPopup() {
  //   Alert(
  //     context: context,
  //     type: AlertType.info,
  //     title: "Login Out",
  //     desc: "Do you want to login out now?",
  //     buttons: [
  //       DialogButton(
  //         child: Text(
  //           "No",
  //           style: TextStyle(color: Colors.white, fontSize: 20),
  //         ),
  //         onPressed: () => Navigator.pop(context),
  //         color: Colors.teal,
  //       ),
  //       DialogButton(
  //         child: Text(
  //           "Yes",
  //           style: TextStyle(color: Colors.white, fontSize: 20),
  //         ),
  //         onPressed: MyApp,
  //         color: Colors.teal,
  //       )
  //     ],
  //   ).show();
  // }

  // Future<Null> handleSignOut() async {
  //   this.setState(() {
  //     isLoading = true;
  //   });

  //   await FirebaseAuth.instance.signOut();
  //   // await googleSignIn.signOut();

  //   this.setState(() {
  //     isLoading = false;
  //   });

    // Navigator.of(context).pushAndRemoveUntil(
    //     MaterialPageRoute(builder: (context) => MyApp()),
    //     (Route<dynamic> route) => false);
  }
// }

