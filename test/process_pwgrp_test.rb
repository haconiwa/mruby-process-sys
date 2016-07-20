assert("Process::Pwgrp.getpwnam") do
  pw = Process::Pwgrp.getpwnam("root")
  assert_equal 0, pw

  pwary = Process::Pwgrp.getpwnam("root", true)
  assert_equal 0, pwary[0]
  assert_equal 0, pwary[1]
  assert_equal "root", pwary[2]
end

assert("Process::Pwgrp.getpwuid") do
  pw = Process::Pwgrp.getpwuid(0)
  assert_equal "root", pw

  pwary = Process::Pwgrp.getpwuid(0, true)
  assert_equal 0, pwary[0]
  assert_equal 0, pwary[1]
  assert_equal "root", pwary[2]
end
