$(document).ready(function () {
    $.ajax({
      url: "http://localhost:8001/api/data",
      type: "GET",
      dataType: "json",
      success: function (data) {
        $("#state").text(data[0].state);
        $("#l1").text(data[0].l1 == true ? "ON" : "OFF");
        $("#l2").text(data[0].l2 == true ? "ON" : "OFF");
        $("#l3").text(data[0].l3);
        $("#l4").text(data[0].l4);
        $("#temp").text(data[0].temp);
        $("#light").text(data[0].light);
      },
      error: function (data) {
        console.log(data);
      }
    });

    setTimeout(function(){
        location.reload();
     }, 3000);
  })