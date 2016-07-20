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

assert("Process::Pwgrp.getgrnam") do
  gr = Process::Pwgrp.getgrnam("root")
  assert_equal 0, gr

  grary = Process::Pwgrp.getgrnam("root", true)
  assert_equal 0, grary[0]
  assert_equal "root", grary[1]
end

assert("Process::Pwgrp.getgrgid") do
  gr = Process::Pwgrp.getgrgid(0)
  assert_equal "root", gr

  grary = Process::Pwgrp.getgrgid(0, true)
  assert_equal 0, grary[0]
  assert_equal "root", grary[1]
end
