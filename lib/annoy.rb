# frozen_string_literal: true

require 'annoy/version'
require 'annoy/annoy'

module Annoy
  class AnnoyIndex
    attr_reader :n_features

    attr_reader :metric

    def initialize(n_features:, metric: 'angular')
      raise ArgumentError, 'Expect n_features to be Integer.' unless n_features.is_a?(Numeric)

      @n_features = n_features.to_i
      @metric = metric

      @index = case @metric
               when 'angular'
                 AnnoyIndexAngular.new(@n_features)
               when 'dot'
                 AnnoyIndexDotProduct.new(@n_features)
               when 'hamming'
                 AnnoyIndexHamming.new(@n_features)
               when 'euclidean'
                 AnnoyIndexEuclidean.new(@n_features)
               when 'mahattan'
                 AnnoyIndexManhattan.new(@n_features)
               else
                 raise ArgumentError, "No such metric: #{@metric}."
               end
    end

    def add_item(i, v)
      @index.add_item(i, v)
    end

    def build(n_trees)
      @index.build(n_trees)
    end

    def save(filename, prefault: false)
      @index.save(filename, prefault)
    end

    def load(filename, prefault: false)
      @index.load(filename, prefault)
    end

    def unload
      @index.unload
    end

    def get_nns_by_item(i, n, search_k: -1, include_distances: false)
      @index.get_nns_by_item(i, n, search_k, include_distances)
    end

    def get_nns_by_vector(v, n, search_k: -1, include_distances: false)
      @index.get_nns_by_vector(v, n, search_k, include_distances)
    end

    def get_item(i)
      @index.get_item(i)
    end

    def get_distance(i, j)
      @index.get_distance(i, j)
    end

    def n_items
      @index.get_n_items
    end

    def n_trees
      @index.get_n_trees
    end

    def on_disk_build(_filename)
      @index.on_disk_build
    end

    def verbose(flag)
      @index.verbose(flag)
    end

    def seed(s)
      @index.set_seed(s)
    end
  end
end
