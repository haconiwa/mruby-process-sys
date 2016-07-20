# mruby-process-sys   [![Build Status](https://travis-ci.org/haconiwa/mruby-process-sys.svg?branch=master)](https://travis-ci.org/haconiwa/mruby-process-sys)
Process class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'haconiwa/mruby-process-sys'
end
```
## example
```ruby
p Process.hi
#=> "hi!!"
t = Process.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the GPL v3 License:
- see LICENSE file
