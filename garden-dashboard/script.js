$(document).ready(function () {

    var xhttp = new XMLHttpRequest();
    var url = "https://localhost:8080/api/data";

    xhttp.onreadystatechange = function(){
        if (this.readyState == 4 && this.status == 200) {
            var arr = JSON.parse(this.responseText);
            $("#l1").text(arr[0].l1 == true ? "ON" : "OFF");
            $("#l2").text(arr[0].l2 == true ? "ON" : "OFF");
            $("#l3").text(arr[0].l3);
            $("#l4").text(arr[0].l4);
            $("#temp").text(arr[0].temp + " °C");
            $("#light").text(arr[0].light);
        }
    };

    xhttp.open("GET", url, true);
    xhttp.send();

});