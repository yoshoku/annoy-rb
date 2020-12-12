# Annoy.rb

[![Build Status](https://github.com/yoshoku/annoy.rb/workflows/build/badge.svg)](https://github.com/yoshoku/annoy.rb/actions?query=workflow%3Abuild)
[![Gem Version](https://badge.fury.io/rb/annoy-rb.svg)](https://badge.fury.io/rb/annoy-rb)
[![License](https://img.shields.io/badge/License-Apache%202.0-yellowgreen.svg)](https://github.com/yoshoku/annoy.rb/blob/main/LICENSE.txt)
[![Documentation](http://img.shields.io/badge/api-reference-blue.svg)](https://yoshoku.github.io/annoy.rb/doc/)

Annoy.rb is a Ruby binding for the [Annoy (Approximate Nearest Neighbors Oh Yeah)](https://github.com/spotify/annoy).

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'annoy-rb'
```

And then execute:

    $ bundle install

Or install it yourself as:

    $ gem install annoy-rb

Note: Annoy.rb does not require the installation of another external library.

## Documentation

* [Annoy.rb API Documentation](https://yoshoku.github.io/annoy.rb/doc/)

## Usage

```ruby
require 'annoy'

f = 40 # length of item vector that will be indexed.
t = Annoy::AnnoyIndex.new(n_features: f, metric: 'angular')

1000.times do |i|
  v = Array.new(f) { rand }
  t.add_item(i, v)
end

t.build(10) # 10 trees.
t.save('test.ann')

u = Annoy::AnnoyIndex.new(n_features: f, metric: 'angular')
u.load('test.ann')
p u.get_nns_by_item(0, 100) # will find the 100 nearest neighbors.
```

## License

The gem is available as open source under the terms of the [Apache-2.0 License](https://www.apache.org/licenses/LICENSE-2.0).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/yoshoku/annoy.rb. This project is intended to be a safe, welcoming space for collaboration, and contributors are expected to adhere to the [code of conduct](https://github.com/yoshoku/annoy.rb/blob/main/CODE_OF_CONDUCT.md).

## Code of Conduct

Everyone interacting in the Annoy.rb project's codebases, issue trackers, chat rooms and mailing lists is expected to follow the [code of conduct](https://github.com/yoshoku/annoy.rb/blob/main/CODE_OF_CONDUCT.md).
