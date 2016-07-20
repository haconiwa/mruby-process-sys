def assert_callable(method_name)
  assert("Process::Sys##{method_name}") do
    t = Process::Sys
    assert_true(t.send(method_name) > 0)
  end
end

assert_callable :getuid
assert_callable :getgid
assert_callable :geteuid
assert_callable :getegid
