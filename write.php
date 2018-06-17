<?php
 include 'keeneye_db.php';
 @mysqli_query ($db, 'set character_set_results = "utf8"');
 //mysql_query ($db, "set character_set_results = 'utf8'");
 $car_id=$_GET['car_id'];
 $data=date('Y-m-d H:i:s');
 $alt=$_GET['alt'];
 $ttff=$_GET['TTFF'];
 $sat=$_GET['sat'];
 $spd=$_GET['spd'];
 $curs=$_GET['curs'];

 $llat=$_GET['lat'];
 $llon=$_GET['lon'];
 $degWhole = intval($llon/100);
 $degDec = ($llon - $degWhole*100)/60;
 $lon = $degWhole + $degDec;
 //echo $lon;
 $degWhole = intval($llat/100);
 $degDec = ($llat - $degWhole*100)/60;
 $lat = $degWhole + $degDec;
 
 $query="INSERT into tracker SET longitude=".$lon.", latitude=".$lat.", car_id=".$car_id.", altitude=".$alt.", speedOTG=".$spd.", course=".$curs;
// $res = mysqli_query($db, $query);

 if(!mysqli_query($db, $query))
    echo "error=".$query;
  else
    echo "OK";
?>
