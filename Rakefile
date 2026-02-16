# frozen_string_literal: true

require 'bundler/gem_tasks'
require 'rspec/core/rake_task'

RSpec::Core::RakeTask.new(spec: :compile)
if ENV['BUNDLE_WITH'] == 'memcheck'
  require 'ruby_memcheck'
  require 'ruby_memcheck/rspec/rake_task'

  namespace :spec do
    RubyMemcheck::RSpec::RakeTask.new(valgrind: :compile)
  end
end

require 'rubocop/rake_task'

RuboCop::RakeTask.new

require 'rake/extensiontask'

task build: :compile # rubocop:disable Rake/Desc

desc 'Run clang-format'
task 'clang-format' do
  sh 'clang-format -style=file -Werror --dry-run ext/annoy/annoyext.*'
end

Rake::ExtensionTask.new('annoyext') do |ext|
  ext.ext_dir = 'ext/annoy'
  ext.lib_dir = 'lib/annoy'
end

task default: %i[clobber compile spec]
