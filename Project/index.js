import { initializeApp } from "https://www.gstatic.com/firebasejs/9.8.1/firebase-app.js";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
const firebaseConfig = {
    apiKey: "AIzaSyBQEh3JRtfHdsK0uq6TtyvrL-CTuVjp7Uk",
    authDomain: "esp32-firebase-demo-1c538.firebaseapp.com",
    databaseURL: "https://esp32-firebase-demo-1c538-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "esp32-firebase-demo-1c538",
    storageBucket: "esp32-firebase-demo-1c538.appspot.com",
    messagingSenderId: "764880000354",
    appId: "1:764880000354:web:a20d479dca78aaf620083b"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

import { getDatabase, ref, get, set, child, update, remove }
    from "https://www.gstatic.com/firebasejs/9.8.1/firebase-database.js"

const db = getDatabase();


let startbtn = document.getElementById("start");
let alert1 = document.getElementById("alert");

let enrollbtn = document.getElementById("enroll");

let fetchbtn = document.getElementById("fetch");

let date1 = "";

startbtn.addEventListener('click', function () {
    // console.log(alert1.innerHTML);
    // console.log(alert1.innerText);

    if (startbtn.innerHTML === 'Start') {

        update(ref(db, "Control/"), {
            "start": true
        }).then(() => {

        }).catch((error) => {
            alert("Error : " + error);
            return;
        })

        date1 = ""

        const d = new Date();

        date1 += d.getDate().toString()
        date1 += " : "
        date1 += (d.getMonth() + 1).toString()
        date1 += " : "
        date1 += d.getFullYear().toString();

        alert1.innerHTML = "starting new session for date : " + date1;

        alert1.classList.remove("d-none");
        alert1.classList.add("show");

        // -------add column--------------

        var tblHeadObj = document.getElementById("table1").tHead;
        for (var h = 0; h < tblHeadObj.rows.length; h++) {
            var newTH = document.createElement('th');
            tblHeadObj.rows[h].appendChild(newTH);
            newTH.innerHTML = date1;
        }

        var tblBodyObj = document.getElementById("table1").tBodies[0];
        for (var i = 0; i < tblBodyObj.rows.length; i++) {
            var newCell = tblBodyObj.rows[i].insertCell(-1);
            newCell.innerHTML = ''
        }

        // console.log(startbtn.innerHTML);
        startbtn.innerHTML = 'End';

        enrollbtn.disabled = true;
        document.getElementById("save").disabled = true;

        // console.log(document.getElementById("table1").innerHTML);
    }
    else if (startbtn.innerHTML === 'End') {
        update(ref(db, "Control/"), {
            "start": false
        }).then(() => {
        }).catch((error) => {
            alert("Error : " + error);
            return;
        })

        alert1.innerHTML = "Session ended!";

        startbtn.innerHTML = 'Start';

        enrollbtn.disabled = false;
        document.getElementById("save").disabled = false;
    }
})

let str1

enrollbtn.addEventListener('click', function () {

    str1 = document.getElementById("input1").value;

    update(ref(db, "Control/"), {
        "name_tmp": str1,
        "enroll": true
    }).then(() => {
    }).catch((error) => {
        alert("Error : " + error);
        return;
    })

    alert1.innerHTML = "Now enrolling face on camera!";
    let num1 = -1;

    // setTimeout(, 20000);
})


fetchbtn.addEventListener('click', function () {

})

document.getElementById("reset").addEventListener('click', function () {


    update(ref(db, "Control/"), {
        'enroll': false,
        'matched_id': -1,
        'name_tmp': "",
        'start': false,
        'table_html': ""
    }).then(() => {
        // alert("student data successfully updated");
        alert1.innerHTML = "database reset!!";
    }).catch((error) => {
        alert("Error : " + error);
        return;
    })

    // remove(ref(db, "Index/")).then(() => {
    //     // alert("student data successfully removed");
    //   }).catch((error) => {
    //     alert("Error : " + error);
    //   })
    // }
})


document.getElementById("save").addEventListener('click', function () {
    let num1
    const dbref = ref(db);

    get(child(dbref, "Control/")).then((snapshot) => {
        if (snapshot.exists()) {
            num1 = snapshot.val()["matched_id"];
        }
        else {
            alert("No data found");
        }
        alert("id = " + num1 + ", name: " + str1);
    }).catch((error) => {
        alert("Error : " + error);
        // return;
    })


    if (num1 >= 0) {
        alert1.innerHTML = "face enrolled for : " + str1 + ", id = " + num1;
        console.log("in alert.inner html!!")
    }
    else if (num1 < 0) {
        alert1.innerHTML = "error enrolling face, face not enrolled in time, try again";
        return;
    }

    set(ref(db, "Index/"), {
        num1: str1
    }).then(() => {
        // alert("student data successfully stored");
    }).catch((error) => {
        alert("Error : " + error);
        return;
    })

    let table = document.getElementById("table1");
    row = table.insertRow(-1);
    row.insertCell(0).innerHTML = str1;
})

