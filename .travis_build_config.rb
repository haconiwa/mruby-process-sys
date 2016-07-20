MRuby::Build.new do |conf|
  toolchain :gcc
  conf.gembox 'default'
  conf.gem '../mruby-process-sys'

  conf.gem github: 'haconiwa/mruby-exec'

  conf.enable_test
end
