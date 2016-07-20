assert("Process#hello") do
  t = Process::Sys
  assert_equal("Process::Sys !!1", t.hello)
end
