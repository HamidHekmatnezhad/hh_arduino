<?php
include ('connection.php');

$sql_insert = "INSERT INTO control_send (flag) VALUES (0)";

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