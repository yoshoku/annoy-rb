# frozen_string_literal: true

require 'bundler/gem_tasks'
require 'rspec/core/rake_task'

RSpec::Core::RakeTask.new(:spec)

require 'rubocop/rake_task'

RuboCop::RakeTask.new

require 'rake/extensiontask'

task build: :compile # rubocop:disable Rake/Desc

Rake::ExtensionTask.new('annoyext') do |ext|
  ext.ext_dir = 'ext/annoy'
  ext.lib_dir = 'lib/annoy'
end

task default: %i[clobber compile rubocop spec]
