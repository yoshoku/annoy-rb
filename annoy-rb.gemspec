# frozen_string_literal: true

require_relative 'lib/annoy/version'

Gem::Specification.new do |spec|
  spec.name          = 'annoy-rb'
  spec.version       = Annoy::VERSION
  spec.authors       = ['yoshoku']
  spec.email         = ['yoshoku@outlook.com']

  spec.summary       = 'Ruby bindings for the Annoy (Approximate Nearest Neighbors Oh Yeah).'
  spec.description   = 'Annoy.rb provides Ruby bindings for the Annoy (Approximate Nearest Neighbors Oh Yeah).'
  spec.homepage      = 'https://github.com/yoshoku/annoy-rb'
  spec.license       = 'Apache-2.0'

  spec.metadata['homepage_uri'] = spec.homepage
  spec.metadata['source_code_uri'] = spec.homepage
  spec.metadata['changelog_uri'] = 'https://github.com/yoshoku/annoy-rb/blob/main/CHANGELOG.md'
  spec.metadata['documentation_uri'] = 'https://yoshoku.github.io/annoy-rb/doc/'
  spec.metadata['rubygems_mfa_required'] = 'true'

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files = Dir.chdir(File.expand_path(__dir__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
                     .select { |f| f.match(/\.(?:rb|rbs|h|hpp|c|cpp|md|txt)$/) }
  end
  spec.files << 'ext/annoy/src/LICENSE'

  spec.bindir        = 'exe'
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ['lib']
  spec.extensions    = ['ext/annoy/extconf.rb']
end
