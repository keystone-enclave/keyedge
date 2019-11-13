#!/usr/bin/expect

set filename [lindex $argv 0]

set timeout 60
spawn ssh -o "UserKnownHostsFile /dev/null" root@localhost -p $::env(HOST_PORT)
expect "yes/no" {
  send "yes\r"
  expect "*?assword" { send "sifive\r" }
  } "*?assword" { send "sifive\r" }
  
expect "# " { send "insmod keystone-driver.ko\r" }
log_file -noappend output.log
expect "# " { send "./$filename/$filename.ke\r" }

expect "# " {
  log_file
  send "poweroff\r" 
  }

expect eof

