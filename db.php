<?php

//connection function
function connectionToSql(){
    //connect to the database
    $db_name = "id19986393_sensordata";
    $user_name = "id19986393_sensorgoesbrr";
    $password = "Projectwork24680.";
    $server_name = "localhost";
    $conn = mysqli_connect($server_name, $user_name, $password, $db_name);
    if (!$conn) {
        return mysqli_connect_error();
    }else{
        return $conn;
    }

}
?>