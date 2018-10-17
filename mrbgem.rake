MRuby::Gem::Specification.new('mruby-process-sys') do |spec|
  spec.license = 'MIT'
  spec.authors = 'Uchio Kondo'
  spec.version = '0.1.1'

  if File.exist? "#{MRUBY_ROOT}/mrbgems/mruby-metaprog"
    spec.add_dependency 'mruby-metaprog', core: 'mruby-metaprog'
  end
  spec.add_dependency 'mruby-process'
end
