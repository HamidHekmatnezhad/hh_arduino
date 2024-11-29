<?php
include ('connection.php');

$sql_insert = "INSERT INTO log (sens_1, sens_2, sens_3, sens_4, sens_5) VALUES ('".$_GET["sens_1"]."', '".$_GET["sens_2"]."', '".$_GET["sens_3"]."', '".$_GET["sens_4"]."', '".$_GET["sens_5"]."')";

if(mysqli_query($con,$sql_insert))
{
echo "Done";
mysqli_close($con);
}
else
{
echo "error is ".mysqli_error($con );
}
?>