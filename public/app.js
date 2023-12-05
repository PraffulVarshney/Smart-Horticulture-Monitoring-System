// //import {getDatabase, ref} from ""

import { initializeApp } from "firebase/app";
import { getDatabase } from "firebase/database";

// // TODO: Replace the following with your app's Firebase project configuration
// // See: https://firebase.google.com/docs/web/learn-more#config-object
const firebaseConfig = {
  apiKey: "AIzaSyDZrwJDY7-V8-MgP52l5RcwvcZIE6vcYYk",
  authDomain: "smarthorticulturesystem.firebaseapp.com",
  databaseURL: "https://smarthorticulturesystem-default-rtdb.asia-southeast1.firebasedatabase.app/",
  projectId: "smarthorticulturesystem",
  storageBucket: "smarthorticulturesystem.appspot.com",
  messagingSenderId: "697071970114",
  appId: "1:697071970114:web:8af5e3f935fe9439198adb",
  measurementId: "G-RHRECH3P6Q"
};

// // Initialize Firebase
// const app = initializeApp(firebaseConfig);


// // Initialize Realtime Database and get a reference to the service
// const database = getDatabase(app);


// import { getDatabase } from "firebase/database";
// const database = getDatabase();

const app= initializeApp(firebaseConfig);
var db = getDatabase(app);


var dataFloatPathH = 'humi/float';
var dataFloatPathT = 'temp/float';
var dataFloatPathM = 'moisture/float';



// Get a database reference 
const databaseFloatH = db.ref(dataFloatPathH);
const databaseFloatT = db.ref(dataFloatPathT);
const databaseFloatM = db.ref(dataFloatPathM);


// Variables to save database current values
var floatReadingT;
var floatReadingH;
var floatReadingM;

// const d = getDatabase();
const starCountRef = ref(db, 'humi/float');
onValue(starCountRef, (snapshot) => {
  const data = snapshot.val();
  console.log(data);
});


// Attach an asynchronous callback to read the data
databaseFloatH.on('value', (snapshot) => {
  floatReadingH = snapshot.val();
  console.log()
  console.log(floatReadingH);
  document.getElementById("reading-floatH").innerHTML = floatReadingH;
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});

databaseFloatT.on('value', (snapshot) => {
  floatReadingT = snapshot.val();
  console.log(floatReadingT);
  document.getElementById("reading-floatT").innerHTML = floatReadingT;
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});

databaseFloatM.on('value', (snapshot) => {
  floatReadingM = snapshot.val();
  console.log(floatReadingM);
  document.getElementById("reading-floatM").innerHTML = floatReadingM;
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});

