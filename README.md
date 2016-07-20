# mruby-process-sys   [![Build Status](https://travis-ci.org/haconiwa/mruby-process-sys.svg?branch=master)](https://travis-ci.org/haconiwa/mruby-process-sys)

Process::Sys (UID/GID) for mruby

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
p Process::Sys.getuid
#=> 1000
p Process::Sys.getgid
#=> 1000

include Process

UID.eid
#=> 1000
UID.from_name "haconiwa"
#=> 1001
#...
```

See Ruby reference manual: [`Process::Sys`](http://docs.ruby-lang.org/ja/2.3.0/class/Process=3a=3aSys.html) [`Process::UID`](http://docs.ruby-lang.org/ja/2.3.0/class/Process=3a=3aUID.html) [`Process::GID`](http://docs.ruby-lang.org/ja/2.3.0/class/Process=3a=3aGID.html)
and `test` directory.

## License

MIT
